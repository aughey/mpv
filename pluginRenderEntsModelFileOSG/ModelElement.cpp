/** <pre>
 *  Entity scene graph plugin
 *  Copyright (c) 2007 Andrew Sampson
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  This class is responsible for loading model files from disk, and presenting 
 *  the resulting scene graph nodes in the form of an "entity element".
 *  
 *  Initial Release: 2007-03-24 Andrew Sampson
 * </pre>
 */


#include <iostream>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/Sequence>
#include <osg/Switch>
#include <osgSim/MultiSwitch>
#include <osg/PositionAttitudeTransform>
#include <osgSim/DOFTransform>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>

#include "ModelElement.h"
#include "MiscOSG.h"

#include "SequenceNodeAnimationImp.h"
#include "SwitchNodeComponentImp.h"
#include "TransformNodeArticulationImp.h"


using namespace mpv;
using namespace mpvosg;


ModelElement::ModelElement( ModelCache *cache ) : EntityElement(), modelCache(cache)
{
	groupNode = new osg::Group;
}

ModelElement::~ModelElement()
{
	
}


bool ModelElement::construct( DefFileGroup *config, Entity *entity )
{

	DefFileAttrib *attr = config->getAttribute( "filename" );
	if( attr )
	{
		std::string modelname = attr->asString();

		osg::Node *cachedNode = modelCache->get( modelname );
		if( cachedNode == NULL )
		{
			// not found in cache
			cachedNode = osgDB::readNodeFile( modelname );
			if( cachedNode )
			{
				modelCache->add( modelname, cachedNode );
			}
		}
		copyAndAttachModelNode( config, entity, cachedNode );
	}
	return true;
}


void ModelElement::copyAndAttachModelNode( DefFileGroup *config, Entity *entity, osg::Node *cachedNode )
{

	osg::ref_ptr< osg::Node > modelNode;
	/*
	Our cache object keeps a pristine copy of the model in memory.  
	This pristine instance can not be used directly, however.  
	Unless a copy is made, all the entities of the same type will 
	share the same switch and articulation nodes.  This would look 
	really silly.

	To fix this, a copy is made.  The model nodes are duplicated, 
	but the textures and vertex arrays are not.  
	
	May want to remove DEEP_COPY_PRIMITIVES from the list below.  
	Not sure what scene elements fall in that category, though.
	*/
	if( cachedNode != NULL )
		// Currently, everything is copied except textures and geometry arrays 
		modelNode = (osg::Node *)cachedNode->clone(
			osg::CopyOp::DEEP_COPY_OBJECTS |
			osg::CopyOp::DEEP_COPY_NODES |
			osg::CopyOp::DEEP_COPY_STATESETS |
			osg::CopyOp::DEEP_COPY_STATEATTRIBUTES |
			osg::CopyOp::DEEP_COPY_PRIMITIVES |
			osg::CopyOp::DEEP_COPY_SHAPES );
			// (to copy textures, "DEEP_COPY_TEXTURES" and "DEEP_COPY_IMAGES" 
			// would be specified)
			// (to copy vertex arrays and similar, "DEEP_COPY_DRAWABLES" and 
			// "DEEP_COPY_ARRAYS" would be specified)
	
	if( !modelNode.valid() )
	{
		// The model file couldn't be loaded... use the default model instead.
		modelNode = createDefaultModel();
	}

	// Set up animation stuff for this model...

	// first, disable DOF node animation (the default behavior for OSG's 
	// "DOFTransform" class is, unfortunately, to have every DOF transform to 
	// constantly wiggle back and forth over its extents of travel... I don't 
	// know *anyone* who wants that as the default behavior for such a node.  
	// IMHO, not a good decision on the part of the OSG maintainers.)
	
	// find all of the DOF nodes in the node tree and enable or disable animation
	std::list<osg::Node *> dofList;
	searchForNodeByClass( modelNode.get(), "DOFTransform", dofList );
	std::list<osg::Node *>::iterator iter;
	for( iter = dofList.begin(); iter != dofList.end(); iter++ )
	{
		osgSim::DOFTransform *dof = dynamic_cast<osgSim::DOFTransform*>( *iter );
		dof->setAnimationOn( false );
	}

	// next, the host-controllable animations are set up
	
	// retrieve (or create) the entity's default animation
	Animation *animation = entity->findOrCreateAnimation( 0 );
	
	// find all the sequence nodes in the model and tie them to a CIGI entity 
	// animation
	std::list<osg::Node *> seqList;
	searchForNodeByClass( modelNode.get(), "Sequence", seqList );
	for( iter = seqList.begin(); iter != seqList.end(); iter++ )
	{
		animation->addImplementation( 
			new SequenceNodeAnimationImp( animation, (osg::Sequence*)(*iter) ) );
	}
	
	std::list<DefFileGroup *>::iterator groupIter;
	
	// articulated parts
	for( groupIter = config->children.begin(); 
		groupIter != config->children.end(); groupIter++ )
	{
		DefFileGroup *artPartGroup = *groupIter;
		if( artPartGroup->getName() == "built_in_articulated_part" )
			constructTransformArtPart( artPartGroup, entity, modelNode.get() );
	}

	// switchable components
	for( groupIter = config->children.begin(); 
		groupIter != config->children.end(); groupIter++ )
	{
		DefFileGroup *switchGroup = *groupIter;
		if( switchGroup->getName() == "built_in_switch" )
			constructSwitchComponentCtrl( switchGroup, entity, modelNode.get() );
	}
	
	
	groupNode->addChild( modelNode.get() );
}


bool ModelElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	groupNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}


void ModelElement::constructTransformArtPart( 
	DefFileGroup *config, Entity *entity, osg::Node *modelNode )
{
	if( config == NULL || entity == NULL || modelNode == NULL )
		return;
	
	int articulationID = -1;
	osg::Node *transformNode = NULL;
	DefFileAttrib *attr = NULL;

	
	attr = config->getAttribute( "art_part_id" );
	if( attr )
		articulationID = attr->asInt();
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - articulated part is missing its \"art_part_id\" attribute\n";
		return;
	}
	

	attr = config->getAttribute( "node_name" );
	if( attr )
	{
		transformNode = searchForNodeByName( modelNode, attr->asString() );
		if( transformNode == NULL )
		{
			std::cout << "Warning - in definition for entity type " 
				<< entity->getType()
				<< " - in articulated part " << articulationID 
				<< ", couldn't find a node in the model scene graph with the name \""
				<< attr->asString() << "\"\n";
			return;
		}
	}
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - articulated part " << articulationID 
			<< " is missing its \"node_name\" attribute\n";
		return;
	}
	
	// make sure that the node is a transform node
	if( !( transformNode && transformNode->asTransform() ) ) 
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - in articulated part " << articulationID 
			<< ", the transform node specified by \"node_name\" "
			<< "does not appear to be a osg::Transform or similar node\n";
		return;
	}

	
	Articulation *articulation = entity->findOrCreateArticulation( articulationID );
	// check to see if the articulation was just created
	if( articulation->getEntityID() != entity->getID() )
	{
		articulation->setEntityID( entity->getID() );
	}

	// If the user has specified default values for the articulation's 
	// transform, then honor those values.
	// Note that this has the potential for being a bit tricky:
	// - if the articulation already existed, then the values specified here
	//   will clobber those already set for the articulation
	// - if the user has multiple articulated part sections for the same 
	//   articulation id, and has provided conflicting default values for 
	//   those sections, then the results will be unpredictable
	attr = config->getAttribute( "rotation" );
	if( attr )
	{
		// FIXME
	}

	attr = config->getAttribute( "offset" );
	if( attr )
	{
		// FIXME
	}
	
	articulation->addImplementation( 
		new TransformNodeArticulationImp( articulation, transformNode->asTransform() ) );
}


void ModelElement::constructSwitchComponentCtrl( 
	DefFileGroup *config, Entity *entity, osg::Node *modelNode )
{
	if( config == NULL || entity == NULL || modelNode == NULL )
		return;
	
	int componentID = -1;
	osg::Node *switchNode = NULL;
	DefFileAttrib *attr = NULL;

	
	attr = config->getAttribute( "component_id" );
	if( attr )
		componentID = attr->asInt();
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - switch component is missing its \"component_id\" attribute\n";
		return;
	}
	

	attr = config->getAttribute( "node_name" );
	if( attr )
	{
		switchNode = searchForNodeByName( modelNode, attr->asString() );
		if( switchNode == NULL )
		{
			std::cout << "Warning - in definition for entity type " 
				<< entity->getType()
				<< " - in switch component " << componentID 
				<< ", couldn't find a node in the model scene graph with the name \""
				<< attr->asString() << "\"\n";
			return;
		}
	}
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - switch component " << componentID 
			<< " is missing its \"node_name\" attribute\n";
		return;
	}
	
	// make sure that the node is a switch node
	if( !( switchNode && switchNode->asGroup() && 
		(dynamic_cast<osg::Switch*>( switchNode ) || 
		 dynamic_cast<osgSim::MultiSwitch*>( switchNode )) ) ) 
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - in switch component " << componentID 
			<< ", the switch node specified by \"node_name\" "
			<< "does not appear to be a osg::Switch or osgSim::MultiSwitch\n";
		return;
	}

	
	Component *component = entity->findOrCreateComponent( componentID );
	// check to see if the component was just created
	if( component->getInstanceID() != entity->getID() )
	{
		component->setInstanceID( entity->getID() );
	}

	SwitchNodeComponentImp *componentImp = 
		new SwitchNodeComponentImp( component, switchNode->asGroup() );
	component->addImplementation( componentImp );

	// Find all the "state" sections.  These sections map a given CIGI value 
	// to a position/child-node of the switch.  I.E. if the Host specifies a 
	// value of N for the component control, the SwitchNodeCompCtrl will use 
	// these state mappings to look up the switch position associated with N, 
	// and will activate that switch position.
	
	std::list<DefFileGroup *>::iterator groupIter;
	for( groupIter = config->children.begin(); 
		groupIter != config->children.end(); groupIter++ )
	{
		DefFileGroup *stateGroup = *groupIter;
		if( stateGroup->getName() == "state" )
		{
			int cigiValue, switchValue;
			attr = stateGroup->getAttribute( "cigi_value" );
			if( attr )
			{
				cigiValue = attr->asInt();
				attr = stateGroup->getAttribute( "switch_value" );
				if( attr )
				{
					switchValue = attr->asInt();
					componentImp->addSwitchStatePair( cigiValue, switchValue );
				}
			}
		}
	}

	// If the user has specified a default value for the component's state, 
	// then honor that value.  The default value is set *after* the 
	// cigi->osgswitch state mappings are processed above.
	// Note that this has the potential for being a bit tricky:
	// - if the component already existed, then the value specified here
	//   will clobber the value already set for the component
	// - if the user has multiple component sections for the same 
	//   component id, and has provided conflicting default values for 
	//   those sections, then the results will be unpredictable
	attr = config->getAttribute( "default_cigi_value" );
	if( attr )
	{
		component->setState( attr->asInt() );
	}

}


// ================================================
// createDefaultModel
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Node * ModelElement::createDefaultModel()
{
	//=========================================================
	//! The default model.  A copy of the default model will be returned if 
	//! there is an error while loading the requested model file.  The default 
	//! model (currently) is a tetrahedron - something that is easily 
	//! identified as "not what we wanted"... This lets the operator know that 
	//! something went wrong while loading the model without bringing down the 
	//! whole system.
	//! 

	osg::Geode *defaultModel = new osg::Geode;
	defaultModel->setName( "default model" );
	osg::Geometry* geometry = new osg::Geometry;
	defaultModel->addDrawable(geometry);
	osg::Vec3Array* vertex = new osg::Vec3Array;
	geometry->setVertexArray(vertex);
	
	double scale = 2.;
	
	// these are the 4 corners of our tetrahedron
	osg::Vec3 vert1(  0.,    0., 6.557 );
	osg::Vec3 vert2(  0., 6.557,   -3. );
	osg::Vec3 vert3( -6.,   -3.,   -3. );
	osg::Vec3 vert4(  6.,   -3.,   -3. );
	
	vert1 *= scale;
	vert2 *= scale;
	vert3 *= scale;
	vert4 *= scale;
	
	// we're making a tetrahedron out of a triangle strip
	vertex->push_back( vert2 );
	vertex->push_back( vert3 );
	vertex->push_back( vert1 );
	vertex->push_back( vert4 );
	vertex->push_back( vert2 );
	vertex->push_back( vert3 );
	
	osg::Vec4Array *colorarray = new osg::Vec4Array;
	colorarray->push_back( osg::Vec4( 1,0,0,1 ) );
	geometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	geometry->setColorArray( colorarray );

	// turn off lighting for this model
	osg::StateSet *stateset = defaultModel->getOrCreateStateSet();
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF );
	
	geometry->addPrimitiveSet(
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,0,vertex->size()));
	geometry->dirtyDisplayList();
	geometry->dirtyBound();

	return defaultModel;
}


