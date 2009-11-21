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
 *  FILENAME:   DefFileAttrib.cpp
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

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include "DefFileAttrib.h"

// ================================================
// DefFileAttrib
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileAttrib::DefFileAttrib()
{
	type_ = ATTRIB_INT;
	setInt( 0 );
}

// ================================================
// ~DefFileAttrib
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileAttrib::~DefFileAttrib()
{
	deleteListItems();
}

// ================================================
// setName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileAttrib::setName( const std::string &name )
{
	name_ = name;
}

// ================================================
// getName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
const std::string & DefFileAttrib::getName() const
{
	return name_;
}
	

// ================================================
// setString
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileAttrib::setString( const std::string &v )
{
	if( type_ == ATTRIB_LIST ) {
		deleteListItems();
	}
	type_ = ATTRIB_STRING;
	stringval = v;
}

// ================================================
// setFloat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileAttrib::setFloat( float v )
{
	if( type_ == ATTRIB_LIST ) {
		deleteListItems();
	}
	type_ = ATTRIB_FLOAT;
	floatval = v;
}

// ================================================
// setInt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileAttrib::setInt( int v )
{
	if( type_ == ATTRIB_LIST ) {
		deleteListItems();
	}
	type_ = ATTRIB_INT;
	intval = v;
}

// ================================================
// appendListItem
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileAttrib::appendListItem( DefFileAttrib* item )
{
	type_ = ATTRIB_LIST;
	list_.push_back( item );
}


// ================================================
// asString
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::string DefFileAttrib::asString() const
{
	std::string result;
	char tempstr[128];
	switch( type_ ) {
		case ATTRIB_STRING:
			result = stringval;
			break;
		case ATTRIB_INT:
			snprintf( tempstr, 127, "%i", intval );
			result = tempstr;
			break;
		case ATTRIB_FLOAT:
			snprintf( tempstr, 127, "%f", floatval );
			result = tempstr;
			break;
		case ATTRIB_LIST:
			{			
			std::list< DefFileAttrib * >::const_iterator iter = list_.begin();
			while( iter != list_.end() )
			{
				result += (*iter)->asString();
				iter++;
				if( iter != list_.end() )
					result += ", ";
			}
			}
			break;
		default:
			break;
	}
	
	return result;
}

// ================================================
// asFloat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
float DefFileAttrib::asFloat() const
{
	float result;
	switch( type_ ) {
		case ATTRIB_STRING:
			result = atof( stringval.c_str() );
			break;
		case ATTRIB_INT:
			result = (float)intval;
			break;
		case ATTRIB_FLOAT:
			result = floatval;
			break;
		case ATTRIB_LIST:
			if( !list_.empty() )
				result = (*list_.begin())->asFloat();
			else
				result = 0.;
			break;
		default:
			result = 0.;
			break;
	}
	
	return result;
}

// ================================================
// asInt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int DefFileAttrib::asInt() const
{
	int result;
	switch( type_ ) {
		case ATTRIB_STRING:
			result = strtol( stringval.c_str(), NULL, 10 );
			break;
		case ATTRIB_INT:
			result = intval;
			break;
		case ATTRIB_FLOAT:
			result = (int)(floatval + 0.5);
			break;
		case ATTRIB_LIST:
			if( !list_.empty() )
				result = (*list_.begin())->asInt();
			else
				result = 0;
			break;
		default:
			result = 0;
			break;
	}

	return result;
}

// ================================================
// asFloats
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::vector<float> DefFileAttrib::asFloats() const
{
	std::vector<float> result;
	
	switch( type_ ) 
	{
		case ATTRIB_STRING:
		case ATTRIB_INT:
		case ATTRIB_FLOAT:
			result.push_back( asFloat() );
			break;
		case ATTRIB_LIST:
		{
			std::list< DefFileAttrib* >::const_iterator iter;
			for( iter = list_.begin(); iter != list_.end(); iter++ )
			{
				result.push_back( (*iter)->asFloat() );
			}
			break;
		}
		default:
			break;
	}
	
	return result;
}

// ================================================
// asInts
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::vector<int> DefFileAttrib::asInts() const
{
	std::vector<int> result;
	
	switch( type_ ) 
	{
		case ATTRIB_STRING:
		case ATTRIB_INT:
		case ATTRIB_FLOAT:
			result.push_back( asInt() );
			break;
		case ATTRIB_LIST:
		{
			std::list< DefFileAttrib* >::const_iterator iter;
			for( iter = list_.begin(); iter != list_.end(); iter++ )
			{
				result.push_back( (*iter)->asInt() );
			}
			break;
		}
		default:
			break;
	}
	
	return result;
}


// ================================================
// print
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileAttrib::print( FILE *fp ) const
{
	std::string value = asString();
	
	// attribute name
	fprintf( fp, "%s = %s;\n", this->name_.c_str(), value.c_str() );
}


// ================================================
// deleteListItems
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DefFileAttrib::deleteListItems()
{
	/* delete elements in list_ */
	if( type_ == ATTRIB_LIST ) {
		for( std::list< DefFileAttrib * >::iterator iter = list_.begin();
			iter != list_.end(); iter++ )
		{
			delete (*iter);
		}
	}

}
