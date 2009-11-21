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
 *  FILENAME:   DefFileParser.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  find def files and feed them into the flex & bison -based parser.
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

#ifndef _DEFINITION_FILE_PARSER_INCLUDED_
#define _DEFINITION_FILE_PARSER_INCLUDED_

#include <string>
#include <list>

#include "DefFileGroup.h"


//==> global variables, defined in DefFileParser.cpp, used by the yacc parser

//=========================================================
//! A pointer to the current group
//!
extern DefFileGroup *currentGroup;

//=========================================================
//! A pointer to the attribute currently being processed
//!
extern DefFileAttrib *tempListAttrib;


//=========================================================
//! The DefFileParser class.  Responsible for finding the def files 
//! and feeding them into the parser.
//!
class MPVCMN_SPEC DefFileParser
{
public:

   //=========================================================
   //! General Constructor
   //!
	DefFileParser();

   //=========================================================
   //! General Destructor
   //!
	~DefFileParser();

	//=========================================================
	//! Parses the specified definition file
	//! \param filename - The name of the definition file to parse
	//! \return a newly-allocated DefFileGroup, or NULL on failure
	//!
	DefFileGroup *parse( const std::string &filename );

};


#endif    //  _DEFINITION_FILE_PARSER_INCLUDED_

