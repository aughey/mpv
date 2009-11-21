#ifndef _MPV_MISC_OSG_H_
#define _MPV_MISC_OSG_H_

#include <string>
#include <list>
#include <osg/Node>
#include <osg/Group>
#include <osg/Matrix>

#include "Mtx4.h"

#include "MPVCommonOSGTypes.h"


//=========================================================
//! Changes parent node of entNode.  A NULL newParent indicates that the 
//! entNode should not be reattached to the scene graph after it has been 
//! removed from its current parents.
//! \param entNode - The node to reparent
//! \param newParent - entNode will be placed under this node.  May be NULL.
//! 
MPVCMNOSG_SPEC void reparentNode( osg::Node *entNode, osg::Group *newParent ) ;

//=========================================================
//! Searches a node tree for any node with a name matching 'name'
//! \param group - The root of the tree to search
//! \param name - The name to look for
//! \return The node with a matching name, or NULL if not found
MPVCMNOSG_SPEC osg::Node * searchForNodeByName( osg::Node *node, const std::string &name );

//=========================================================
//! Searches a node tree for any node with a class name matching 'name'
//! \param group - The root of the tree to search
//! \param name - The class name to look for
//! \param l - A list of nodes with a matching name; if list is empty, no matches were found
MPVCMNOSG_SPEC void searchForNodeByClass( osg::Node *node, const std::string &name, std::list<osg::Node *> &l );

//! Converts a MPV matrix to an OSG matrix
//! \param mpvMtx - The Mtx4 to convert
//! \return An OSG matrix
MPVCMNOSG_SPEC osg::Matrix convertMPVMatrixToOSGMatrix( const mpv::Mtx4 &mpvMtx );

//! Converts an OSG matrix to a MPV matrix
//! \param osgMtx - The Matrix to convert
//! \return A MPV matrix
MPVCMNOSG_SPEC mpv::Mtx4 convertOSGMatrixToMPVMatrix( const osg::Matrix &osgMtx );

#endif
