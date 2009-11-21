/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   DefFileGroup.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   represent a group defined in a definition file.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "DefFileGroup.h"



// ================================================
// DefFileGroup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileGroup::DefFileGroup(  )
{
	parent = NULL;
}

// ================================================
// ~DefFileGroup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileGroup::~DefFileGroup(  )
{

	/* delete attributes */
	for( std::list< DefFileAttrib * >::iterator iterA = this->attributes.begin();
		iterA != this->attributes.end(); iterA++ )
	{
		delete (*iterA);
	}
	
	/* delete children !!! RECURSIVE !!! */
	for( std::list< DefFileGroup * >::iterator iterG = this->children.begin();
		iterG != this->children.end(); iterG++ )
	{
		delete (*iterG);
	}
	
}


// ================================================
// setName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileGroup::setName( const std::string &name )
{
	name_ = name;
}

// ================================================
// getName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const std::string & DefFileGroup::getName() const
{
	return name_;
}


// ================================================
// addChild
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileGroup::addChild( DefFileGroup* child )
{
	
	if( child == NULL ) return;
	
	if( child->parent != NULL )
	{
		// The child was formerly under another parent.  This isn't supposed 
		// to happen, but it might.
		child->parent->children.remove( child );
	}
	
	child->parent = this;
	
	children.push_back( child );
}


// ================================================
// removeChild
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileGroup::removeChild( DefFileGroup *child )
{
	if( child == NULL ) return;
	if( child->parent != this ) return;
	
	children.remove( child );
	child->parent = NULL;
}


// ================================================
// getChild
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileGroup * DefFileGroup::getChild( const std::string &name )
{
	for( std::list< DefFileGroup * >::iterator iterG = this->children.begin();
		iterG != this->children.end(); iterG++ )
	{
		if( (*iterG)->getName() == name ) {
			return (*iterG);
		}
	}
	return NULL;
}


// ================================================
// getChild (const version)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const DefFileGroup * DefFileGroup::getChild( const std::string &name ) const
{
	for( std::list< DefFileGroup * >::const_iterator iterG = this->children.begin();
		iterG != this->children.end(); iterG++ )
	{
		if( (*iterG)->getName() == name ) {
			return (*iterG);
		}
	}
	return NULL;
}


// ================================================
// getGroupByURI
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileGroup * DefFileGroup::getGroupByURI( const std::string &uriStr )
{
	DefFileGroup *result = NULL;
	char * uriCopy = strdup( uriStr.c_str() );
	char * uri = uriCopy;

	// skip leading slashes
	while( *uri == '/' )
		uri++;
	
	// Now search for the next slash.  
	// If there are no more slashes, we've reached the "attribute" part of the 
	// URI.  In such a situation, we set 'result' to 'this'.
	// If there is another slash, we need to search our child groups to see if 
	// any of them match the text between the "beginning" of the string and 
	// the first slash.
	char *tokenStart = uri;
	while( *uri != '/' && *uri != '\0' )
		uri++;
	if( *uri == '\0' )
	{
		// we've reached the "attribute" part of the URI
		result = this;
	}
	else
	{
		// we need to search our child groups
		std::string groupName( tokenStart, uri - tokenStart );
		std::list< DefFileGroup * >::iterator iterG = this->children.begin();
		for( ; iterG != this->children.end(); iterG++ )
		{
			DefFileGroup *child = (*iterG);
			if( child->getName() == groupName )
				result = child->getGroupByURI( uri );
			if( result )
				break;
		}
	}
	
	free( uriCopy );
	return result;
}


// ================================================
// getGroupByURI (const version)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const DefFileGroup * DefFileGroup::getGroupByURI( const std::string &uriStr ) const
{
	const DefFileGroup *result = NULL;
	char * uriCopy = strdup( uriStr.c_str() );
	char * uri = uriCopy;

	// skip leading slashes
	while( *uri == '/' )
		uri++;
	
	// Now search for the next slash.  
	// If there are no more slashes, we've reached the "attribute" part of the 
	// URI.  In such a situation, we set 'result' to 'this'.
	// If there is another slash, we need to search our child groups to see if 
	// any of them match the text between the "beginning" of the string and 
	// the first slash.
	char *tokenStart = uri;
	while( *uri != '/' && *uri != '\0' )
		uri++;
	if( *uri == '\0' )
	{
		// we've reached the "attribute" part of the URI
		result = this;
	}
	else
	{
		// we need to search our child groups
		std::string groupName( tokenStart, uri - tokenStart );
		std::list< DefFileGroup * >::const_iterator iterG = this->children.begin();
		for( ; iterG != this->children.end(); iterG++ )
		{
			const DefFileGroup *child = (*iterG);
			if( child->getName() == groupName )
				result = child->getGroupByURI( uri );
			if( result )
				break;
		}
	}
	
	free( uriCopy );
	return result;
}


// ================================================
// addAttribute
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileGroup::addAttribute( DefFileAttrib * attr )
{
	if( attr == NULL ) return;
	attributes.push_back( attr );
}


// ================================================
// removeAttribute
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileGroup::removeAttribute( DefFileAttrib *attr )
{
	if( attr == NULL ) return;
	attributes.remove( attr );
}


// ================================================
// getAttribute
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileAttrib * DefFileGroup::getAttribute( const std::string &name )
{
	for( std::list< DefFileAttrib * >::iterator iterA = this->attributes.begin();
		iterA != this->attributes.end(); iterA++ )
	{
		if( (*iterA)->getName() == name ) {
			return (*iterA);
		}
	}
	return NULL;
}


// ================================================
// getAttribute (const version)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const DefFileAttrib * DefFileGroup::getAttribute( const std::string &name ) const
{
	for( std::list< DefFileAttrib * >::const_iterator iterA = this->attributes.begin();
		iterA != this->attributes.end(); iterA++ )
	{
		if( (*iterA)->getName() == name ) {
			return (*iterA);
		}
	}
	return NULL;
}


// ================================================
// getAttributeByURI
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileAttrib * DefFileGroup::getAttributeByURI( const std::string &uriStr )
{
	DefFileAttrib *result = NULL;
	char * uriCopy = strdup( uriStr.c_str() );
	char * uri = uriCopy;

	// skip leading slashes
	while( *uri == '/' )
		uri++;
	
	// Now search for the next slash.  If there are no more slashes, we've 
	// reached the "attribute" part of the URI.  If there is another slash, we 
	// need to search our child groups to see if any of them match the text 
	// between the "beginning" of the string and the first slash.
	char *tokenStart = uri;
	while( *uri != '/' && *uri != '\0' )
		uri++;
	if( *uri == '\0' )
	{
		// we've reached the "attribute" part of the URI
		result = getAttribute( tokenStart );
	}
	else
	{
		// we need to search our child groups
		std::string groupName( tokenStart, uri - tokenStart );
		std::list< DefFileGroup * >::iterator iterG = this->children.begin();
		for( ; iterG != this->children.end(); iterG++ )
		{
			DefFileGroup *child = (*iterG);
			if( child->getName() == groupName )
				result = child->getAttributeByURI( uri );
			if( result )
				break;
		}
	}
	
	free( uriCopy );
	return result;
}


// ================================================
// getAttributeByURI (const version)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const DefFileAttrib * DefFileGroup::getAttributeByURI( const std::string &uriStr ) const
{
	const DefFileAttrib *result = NULL;
	char * uriCopy = strdup( uriStr.c_str() );
	char * uri = uriCopy;

	// skip leading slashes
	while( *uri == '/' )
		uri++;
	
	// Now search for the next slash.  If there are no more slashes, we've 
	// reached the "attribute" part of the URI.  If there is another slash, we 
	// need to search our child groups to see if any of them match the text 
	// between the "beginning" of the string and the first slash.
	char *tokenStart = uri;
	while( *uri != '/' && *uri != '\0' )
		uri++;
	if( *uri == '\0' )
	{
		// we've reached the "attribute" part of the URI
		result = getAttribute( tokenStart );
	}
	else
	{
		// we need to search our child groups
		std::string groupName( tokenStart, uri - tokenStart );
		std::list< DefFileGroup * >::const_iterator iterG = this->children.begin();
		for( ; iterG != this->children.end(); iterG++ )
		{
			const DefFileGroup *child = (*iterG);
			if( child->getName() == groupName )
				result = child->getAttributeByURI( uri );
			if( result )
				break;
		}
	}
	
	free( uriCopy );
	return result;
}


// ================================================
// print
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileGroup::print( FILE *fp, int level ) const
{
	int i;
	/* group name */
	for( i=0; i < level; i++ )
		fprintf( fp, "\t" );
	fprintf( fp, "%s\n", this->name_.c_str() );

	/* opening brace */
	for( i=0; i < level; i++ )
		fprintf( fp, "\t" );
	fprintf( fp, "{\n" );

	/* attributes */
	for( std::list< DefFileAttrib * >::const_iterator iterA = this->attributes.begin();
		iterA != this->attributes.end(); iterA++ )
	{
		for( i=0; i < level+1; i++ )
			fprintf( fp, "\t" );
		(*iterA)->print( fp );
	}
	
	/* children */
	for( std::list< DefFileGroup * >::const_iterator iterG = this->children.begin();
		iterG != this->children.end(); iterG++ )
	{
		(*iterG)->print( fp, level + 1 );
	}

	/* closing brace */
	for( i=0; i < level; i++ )
		fprintf( fp, "\t" );
	fprintf( fp, "}\n" );
}


