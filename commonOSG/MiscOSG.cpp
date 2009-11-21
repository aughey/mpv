

#include "MiscOSG.h"

// ================================================
// reparentNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MPVCMNOSG_SPEC void reparentNode( osg::Node *node, osg::Group *newParent ) 
{
	if( !node ) return;

	node->ref();

	// first, remove node from all parents
	while( node->getNumParents() > 0 ) {
		node->getParent(0)->removeChild( node );
	}
	
	// next, add node to the newParent
	if( newParent ) {
		newParent->addChild( node );
	}

	node->unref();

}


// ================================================
// searchForNodeByName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MPVCMNOSG_SPEC osg::Node * searchForNodeByName( osg::Node *node, const std::string &name )
{
	if( node == NULL ) return NULL;

	if( node->getName() == name ) return node;
	
	osg::Group *group = node->asGroup();
	if( group == NULL ) return NULL;
	
	for( unsigned int i = 0; i < group->getNumChildren(); i++ )
	{
		osg::Node *child = group->getChild(i);

		osg::Node *result = searchForNodeByName( child, name );
		if( result ) 
			return result;
	}
	
	// if no children contained the requested node, return NULL
	return NULL;
}


// ================================================
// searchForNodeByClass
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MPVCMNOSG_SPEC void searchForNodeByClass( osg::Node *node, const std::string &name, std::list<osg::Node *> &result )
{
	if( node == NULL ) return;

	if( node->className() == name )
		result.push_back( node );

	osg::Group *group = node->asGroup();
	if( group == NULL ) return;
	
	for( unsigned int i = 0; i < group->getNumChildren(); i++ )
	{
		osg::Node *child = group->getChild(i);
		
		searchForNodeByClass( child, name, result );
	}
	
	return;
}


// ================================================
// convertMPVMatrixToOSGMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MPVCMNOSG_SPEC osg::Matrix convertMPVMatrixToOSGMatrix( const mpv::Mtx4 &mpvMtx )
{
	osg::Matrix result;
	// OSG matrices are stored "sideways", like OpenGL.
	// MPV matrices are stored like you learned in math class (except the subscript is 
	// backwards; math class convention for refering to an element is Mcol,row).
	// They need to be transposed to convert between the two.
	result.set( mpvMtx.transpose().GetM() );
	return result;
}


// ================================================
// convertOSGMatrixToMPVMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MPVCMNOSG_SPEC mpv::Mtx4 convertOSGMatrixToMPVMatrix( const osg::Matrix &osgMtx )
{
	mpv::Mtx4 result;
	// OSG matrices are stored "sideways", like OpenGL.
	// MPV matrices are stored like you learned in math class (except the subscript is 
	// backwards; math class convention for refering to an element is Mcol,row).
	// They need to be transposed to convert between the two.
	result.pcset( osgMtx.ptr() );
	return result;
}

