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
 *  FILENAME:   DefFileAttrib.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   represent an attribute defined in a definition file.
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

#ifndef _DEFINITION_FILE_ATTRIBUTE_INCLUDED_
#define _DEFINITION_FILE_ATTRIBUTE_INCLUDED_

#include <string>
#include <list>
#include <vector>

#include "MPVCommonTypes.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
#endif

//=========================================================
//! An enumeration defining the types of attribute values
//!
enum DefFileAttribType {
	ATTRIB_STRING,
	ATTRIB_INT,
	ATTRIB_FLOAT,
	ATTRIB_LIST

};

//======================================================
//! A class for storing attribute name-value pairs, as loaded from .def files
class MPVCMN_SPEC DefFileAttrib {
public:


	//==> Management

	//============================================
	//! General constructor for DefFileAttrib
	//!
	DefFileAttrib();

	//============================================
	//! General destructor for DefFileAttrib
	//!
	~DefFileAttrib();


	//==> Accessors

	//============================================
	//! Method to set the name of this attribute.  Unless this method is 
	//! called, the attribute's name defaults to an empty string.
	//! \param name - The new name for this attribute
	//!
	void setName( const std::string &name );

	//============================================
	//! Gets the name for this attribute.
	//! \return A string containing the attributes name
	//!
	const std::string & getName() const;
	
	//============================================
	//! Gets this attribute's type.
	//! \return The attribute's type, corresponding to one of the values in the DefFileAttribType enum.
	//!
	int getType() const { return type_; }
	
	//============================================
	//! Sets the value for this attribute to be a string.  This will replace 
	//! any existing value the attribute may have.
	//! \param v - The new string value.
	//!
	void setString( const std::string &v );
	
	//============================================
	//! Sets the value for this attribute to be a float.  This will replace 
	//! any existing value the attribute may have.
	//! \param v - The new float value.
	//!
	void setFloat( float v );
	
	//============================================
	//! Sets the value for this attribute to be an int.  This will replace 
	//! any existing value the attribute may have.
	//! \param v - The new int value.
	//!
	void setInt( int v );
	
	//============================================
	//! Sets this attribute to be a "list" attribute, ie an attribute that 
	//! contains other attributes.  
	//! \param item - The attribute to append to the list.
	//!
	void appendListItem( DefFileAttrib* item );
	
	
	//============================================
	//! Converts (if necessary) the attribute's value to a string and returns 
	//! it.  If the attribute is actually a list, the first entry in the list 
	//! will be converted and returned.
	//! \return The attribute's value, as a string.
	//!
	std::string asString() const;
	
	//============================================
	//! Converts (if necessary) the attribute's value to a float and returns 
	//! it.  If the attribute is actually a list, the first entry in the list 
	//! will be converted and returned.
	//! \return The attribute's value, as a float.
	//!
	float asFloat() const;
	
	//============================================
	//! Converts (if necessary) the attribute's value to an int and returns 
	//! it.  If the attribute is actually a list, the first entry in the list 
	//! will be converted and returned.  Note that if the attribute was 
	//! actually specified as a float in the def file, it will be *rounded* to 
	//! the nearest integer (not truncated) when it is converted.
	//! \return The attribute's value, as an int.
	//!
	int asInt() const;
	
	//============================================
	//! Converts (if necessary) the elements of the list_ member and returns 
	//! those values as a list of floats.  If the attribute is not a list, 
	//! then the attribute's value will be converted to a float and returned 
	//! as a member of the list (ie the list will have a single entry).
	//! \return A list of floats
	//!
	std::vector<float> asFloats() const;
	
	//============================================
	//! Converts (if necessary) the elements of the list_ member and returns 
	//! those values as a list of ints.  If the attribute is not a list, 
	//! then the attribute's value will be converted to an int and returned 
	//! as a member of the list (ie the list will have a single entry).
	//! 
	//! NOTE - List-type attributes are always stored as floats internally; 
	//! when they are retrieved via asInts, they are converted to ints.  For 
	//! this reason, the user should be aware that integer list entries will 
	//! not have the same range of valid values as non-list-type integer 
	//! attributes.  This note applies only to *list* attributes.
	//! 
	//! \return A list of ints
	//!
	std::vector<int> asInts() const;
	
	//============================================
	//! Prints the contents of this attribute.
	//! \param fp - The file pointer to send the output to (defaults to stdout)
	//!
	void print( FILE *fp = stdout ) const;


private:

	//=========================================================
	//! This attribute's name
	//!
	std::string name_;

	//=========================================================
	//! The attribute's value type
	//!
	int type_;

	//=========================================================
	//! For "list"-type attributes, this member stores pointers to 
	//! the elements of the list.
	//!
	std::list< DefFileAttrib* > list_;


	//=========================================================
	//! The attribute's value as an int
	//!
	int intval;

	//=========================================================
	//! The attribute's value as an float
	//!
	float floatval;

	//=========================================================
	//! The attribute's value as an string
	//!
	std::string stringval;


	//=========================================================
	//! Deletes the items in the list.
	//!
	void deleteListItems() ;

};

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif    //  _DEFINITION_FILE_ATTRIBUTE_INCLUDED_


