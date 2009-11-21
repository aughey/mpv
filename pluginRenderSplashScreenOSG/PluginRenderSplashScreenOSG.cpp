/** <pre>
 *  Plugin for the MPV to create a splash screen
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
 *  2007-08-16 Andrew Sampson
 *      Initial implementation
 *
 * </pre>
 */


#include <iostream>
#include <osg/Geode>
#include <osgDB/ReadFile>
#include <osgText/Text>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>

#include "PluginRenderSplashScreenOSG.h"



EXPORT_DYNAMIC_CLASS( PluginRenderSplashScreenOSG )

// ================================================
// PluginRenderSplashScreenOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSplashScreenOSG::PluginRenderSplashScreenOSG() : Plugin() 
{
	name_ = "PluginRenderSplashScreenOSG";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );
	
	splashScreenRoot = new osg::Group;
	splashScreenRoot->setName( "Splash Screen Root" );
	
	// disable lighting for the entire splash screen
	osg::StateSet *stateSet = splashScreenRoot->getOrCreateStateSet();
	stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
}


// ================================================
// ~PluginRenderSplashScreenOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSplashScreenOSG::~PluginRenderSplashScreenOSG() throw() 
{
	// Note - the splash screen root will be destroyed when our reference to it 
	// falls out of scope (ie here...).
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSplashScreenOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard
		
		createSplashScreen();
		
		bb_->put( "SplashScreenRootNodeOSG", splashScreenRoot.get() );
		
		break;

	default:
		break;
	}
	
}


// ================================================
// createSplashScreen
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSplashScreenOSG::createSplashScreen()
{
	osg::Geode *geode = new osg::Geode;
	
	osgText::Text *mpvText = new osgText::Text;
	mpvText->setText( "CIGI Multi-Purpose Viewer" );
	mpvText->setColor( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	mpvText->setCharacterSize( 0.04 );
	mpvText->setFont( 0 );  // brutally ugly default font
	mpvText->setPosition( osg::Vec3( -0.33, 0.0, 0.0 ) );
	mpvText->setAlignment( osgText::Text::LEFT_CENTER );
	
	geode->addDrawable( mpvText );
	
	mpvText = new osgText::Text;
	mpvText->setText( "Waiting for Host" );
	mpvText->setColor( osg::Vec4( 0.7, 0.7, 0.7, 1.0 ) );
	mpvText->setCharacterSize( 0.04 );
	mpvText->setFont( 0 );  // brutally ugly default font
	mpvText->setPosition( osg::Vec3( -0.33, -0.08, 0.0 ) );
	mpvText->setAlignment( osgText::Text::LEFT_CENTER );
	
	geode->addDrawable( mpvText );
	
	splashScreenRoot->addChild( geode );

	osg::PositionAttitudeTransform *logoPosition = new osg::PositionAttitudeTransform;
	logoPosition->setPosition( osg::Vec3d( -0.6, 0.0, 0.0 ) );
	logoPosition->setAttitude( osg::Quat( osg::DegreesToRadians( -15. ), osg::Vec3( 0,0,1 ) ) );
	logoPosition->setScale( osg::Vec3d( 0.2,0.2,0.2 ) );
	splashScreenRoot->addChild( logoPosition );

	// create three squares in red, green, and blue, rotated and superimposed
	osg::PositionAttitudeTransform *rotate;
	osg::PositionAttitudeTransform *translate;
	
	// red
	rotate = new osg::PositionAttitudeTransform;
	translate = new osg::PositionAttitudeTransform;
	rotate->addChild( translate );
	rotate->setAttitude( osg::Quat( osg::DegreesToRadians( 120.0 * 0. ), osg::Vec3( 0,0,1 ) ) );
	translate->setPosition( osg::Vec3d( 0.07735, 0.16666, 0.0 ) );
	translate->addChild( createSquare( osg::Vec4( 1,0,0,1 ) ) );
	logoPosition->addChild( rotate );
	
	// green
	rotate = new osg::PositionAttitudeTransform;
	translate = new osg::PositionAttitudeTransform;
	rotate->addChild( translate );
	rotate->setAttitude( osg::Quat( osg::DegreesToRadians( 120.0 * 1. ), osg::Vec3( 0,0,1 ) ) );
	translate->setPosition( osg::Vec3d( 0.07735, 0.16666, 0.0 ) );
	translate->addChild( createSquare( osg::Vec4( 0,1,0,1 ) ) );
	logoPosition->addChild( rotate );
	
	// blue
	rotate = new osg::PositionAttitudeTransform;
	translate = new osg::PositionAttitudeTransform;
	rotate->addChild( translate );
	rotate->setAttitude( osg::Quat( osg::DegreesToRadians( 120.0 * 2. ), osg::Vec3( 0,0,1 ) ) );
	translate->setPosition( osg::Vec3d( 0.07735, 0.16666, 0.0 ) );
	translate->addChild( createSquare( osg::Vec4( 0,0,1,1 ) ) );
	logoPosition->addChild( rotate );
	
	// white triangle
	translate = new osg::PositionAttitudeTransform;
	// put the triangle on top of the other shapes
	translate->setPosition( osg::Vec3d( 0, 0, 0.1 ) );
	translate->addChild( createTriangle( osg::Vec4( 1,1,1,1 ) ) );
	logoPosition->addChild( translate );
}


// ================================================
// createSquare
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Geode *PluginRenderSplashScreenOSG::createSquare( const osg::Vec4 &color )
{
	// creates a line-art 2D square, 1.0 units on a side, centered at 0,0
	
	osg::Geode *geode = new osg::Geode;
	osg::Geometry *geometry = new osg::Geometry;
	geode->addDrawable( geometry );
	
	osg::Vec3Array* vertexArray = new osg::Vec3Array;
	geometry->setVertexArray( vertexArray );
	
	vertexArray->push_back( osg::Vec3( -0.5, -0.5, 0. ) );
	vertexArray->push_back( osg::Vec3(  0.5, -0.5, 0. ) );
	vertexArray->push_back( osg::Vec3(  0.5,  0.5, 0. ) );
	vertexArray->push_back( osg::Vec3( -0.5,  0.5, 0. ) );
	
	osg::Vec4Array *colorarray = new osg::Vec4Array;
	colorarray->push_back( color );
	geometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	geometry->setColorArray( colorarray );

	geometry->addPrimitiveSet(
		new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, vertexArray->size() ) );
	geometry->dirtyDisplayList();
	geometry->dirtyBound();
	
	return geode;
}


// ================================================
// createTriangle
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Geode *PluginRenderSplashScreenOSG::createTriangle( const osg::Vec4 &color )
{
	// creates a line-art 2D equilateral triangle.  
	// it will be 1.1547 units on a side (making the radius 2/3), centered at 0,0
	
	osg::Geode *geode = new osg::Geode;
	osg::Geometry *geometry = new osg::Geometry;
	geode->addDrawable( geometry );
	
	osg::Vec3Array* vertexArray = new osg::Vec3Array;
	geometry->setVertexArray( vertexArray );
	
	vertexArray->push_back( osg::Vec3( -0.57735, -1./3., 0. ) );
	vertexArray->push_back( osg::Vec3(  0.57735, -1./3., 0. ) );
	vertexArray->push_back( osg::Vec3(  0.00000,  2./3., 0. ) );
	
	osg::Vec4Array *colorarray = new osg::Vec4Array;
	colorarray->push_back( color );
	geometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	geometry->setColorArray( colorarray );

	geometry->addPrimitiveSet(
		new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, vertexArray->size() ) );
	geometry->dirtyDisplayList();
	geometry->dirtyBound();
	
	return geode;
}
