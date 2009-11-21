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
 *  FILENAME:   DefFileParser.cpp
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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "DefFileParser.h"

int yyparse( void );
void flushDefLexer();

extern FILE *yyin;
extern int linenum;

DefFileGroup *currentGroup = NULL;
DefFileAttrib *tempListAttrib = NULL;




// ================================================
// DefFileParser
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileParser::DefFileParser()
{
	
// FIXME - should make this class a singleton, as it uses globals (currentGroup and tempListAttrib)

}


// ================================================
// ~DefFileParser
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileParser::~DefFileParser()
{
}


// ================================================
// parse
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DefFileGroup * DefFileParser::parse( const std::string &filename )
{

	FILE *fp = fopen( filename.c_str(), "r" );

	if( !fp )
		return NULL;

	yyin = fp;

	DefFileGroup *root = new DefFileGroup();
	root->setName( "root group" );

	// initialize globals before starting the parse
	linenum = 1;
	currentGroup = root;
	tempListAttrib = new DefFileAttrib;

	int parseStatus = yyparse();
	
	/* This flush is critical, especially if there was a parse error.  Flex/lex 
	parsers maintain a cached buffer of input, and if there is a parse error 
	then the parse process will halt, leaving un-processed input in the 
	buffer.  If the parser is invoked again, even on a different file, the 
	lexer will use the (stale, riddled-with-syntax-errors) contents of the 
	cached buffer instead of the specified file.  Hooray for global variables! 
	*/
	flushDefLexer();
	
	delete tempListAttrib;
	tempListAttrib = NULL;
	currentGroup = NULL;
	yyin = NULL;
	
	// report parse errors
	if( parseStatus != 0 )
	{
		delete root;
		return NULL;
	}
	
//	root->print();
	
	fclose( fp );
	
	return root;
}


