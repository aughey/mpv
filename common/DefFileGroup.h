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
 *  FILENAME:   DefFileGroup.h
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

#ifndef _DEFINITION_FILE_GROUP_INCLUDED_
#define _DEFINITION_FILE_GROUP_INCLUDED_

#include <stdio.h>
#include <list>
#include <string>

#include "DefFileAttrib.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
#endif

//======================================================
//! A class for storing groups, as loaded from .def files.  Groups can contain 
//! attributes and other groups.
class MPVCMN_SPEC DefFileGroup {
public:


	//==> Management

	//============================================
	//! General constructor for DefFileGroup
	DefFileGroup(  );

	//============================================
	//! General destructor for DefFileGroup
	//!
	~DefFileGroup(  );


	//==> Accessors

	//============================================
	//! Sets the name for this group.
	//! \param name - The new name
	//!
	void setName( const std::string &name );
	
	//============================================
	//! Gets the name for this group.
	//! \return The name for this group
	//!
	const std::string & getName() const;
	
	//============================================
	//! Adds a group to this group as a child.
	//! \param child - The group to add.
	//!
	void addChild( DefFileGroup *child );
	
	//============================================
	//! Removes a group from this group.
	//! \param child - The group to remove.
	//!
	void removeChild( DefFileGroup *child );

	//============================================
	//! Searches this group's list of child groups for one with specified name.
	//! \param name - The name of the group to look for.
	//! \return The first child in the group with the given name, or NULL if not found.
	//!
	DefFileGroup * getChild( const std::string &name );
	
	//============================================
	//! Searches this group's list of child groups for one with specified name.
	//! Const version; \see getChild()
	//! \param name - The name of the group to look for.
	//! \return The first child in the group with the given name, or NULL if not found.
	//!
	const DefFileGroup * getChild( const std::string &name ) const;
	
	//============================================
	//! Retrieves group by URI (Universal Resource Identifier... kind of 
	//! like a URL).  If more than one group in the def tree matches the 
	//! URI, the first matching group encountered will be returned.
	//! Attribute URIs are specified like this: "/group1/attribute" or "/group1/group2/attribute"
	//! Group URIs are specified like this: "/group1/" or "/group1/group2/"
	//! (note the trailing slash)
	//! \param name - The URI of the group to look for.
	//! \return The first group in the tree with the given name, or NULL if not found.
	//!
	DefFileGroup * getGroupByURI( const std::string &uri );
	
	//============================================
	//! Retrieves group by URI (Universal Resource Identifier... kind of 
	//! like a URL).  Const version; \see getGroupByURI()
	//! \param name - The URI of the group to look for.
	//! \return The first group in the tree with the given name, or NULL if not found.
	//!
	const DefFileGroup * getGroupByURI( const std::string &uri ) const;
	
	//============================================
	//! Adds an attribute to this group.
	//! \param attr - The attribute to add.
	//!
	void addAttribute( DefFileAttrib * attr );
	
	//============================================
	//! Removes an attribute from this group.
	//! \param attr - The attribute to remove.
	//!
	void removeAttribute( DefFileAttrib *attr );
	
	//============================================
	//! Searches this group's list of attributes for one with specified name.
	//! \param name - The name of the attribute to look for.
	//! \return The first attribute in the group with the given name, or NULL if attrib not found.
	//!
	DefFileAttrib * getAttribute( const std::string &name );
	
	//============================================
	//! Searches this group's list of attributes for one with specified name.
	//! Const version; \see getAttribute()
	//! \param name - The name of the attribute to look for.
	//! \return The first attribute in the group with the given name, or NULL if attrib not found.
	//!
	const DefFileAttrib * getAttribute( const std::string &name ) const;
	
	//============================================
	//! Retrieves an attribute by URI (Universal Resource Identifier... kind of 
	//! like a URL).  If more than one attribute in the def tree matches the 
	//! uri, the first matching attribute encountered will be returned.
	//! Attribute URIs are specified like this: "/group1/attribute" or "/group1/group2/attribute"
	//! Group URIs are specified like this: "/group1/" or "/group1/group2/"
	//! (note the trailing slash)
	//! \param name - The URI of the attribute to look for.
	//! \return The first attribute in the group with the given name, or NULL if attrib not found.
	//!
	DefFileAttrib * getAttributeByURI( const std::string &uri );
	
	//============================================
	//! Retrieves an attribute by URI (Universal Resource Identifier... kind of 
	//! like a URL).  Const version; \see getAttributeByURI()
	//! \param name - The URI of the attribute to look for.
	//! \return The first attribute in the group with the given name, or NULL if attrib not found.
	//!
	const DefFileAttrib * getAttributeByURI( const std::string &uri ) const;
	
	//============================================
	//! Print the contents of this group (recursive).
	//! \param fp - The file pointer to send the output to (defaults to stdout)
	//! \param level - The level of indentation (defaults to 0)
	//!
	void print( FILE *fp = stdout, int level = 0 ) const;


	//==> public variables

	//=========================================================
	//! Pointer to the group's parent
	//!
	DefFileGroup * parent;
	
	//=========================================================
	//! A list of pointers to the group's children
	//!
	std::list< DefFileGroup * > children;
	
	//=========================================================
	//! A list of pointers to the group's attributes
	//!
	std::list< DefFileAttrib * > attributes;


private:
	
	//=========================================================
	//! The name of this group
	//!
	std::string name_;

};

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif    //  _DEFINITION_FILE_GROUP_INCLUDED_
