/*

*/

%{
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
 *  FILENAME:   deffile-yacc.yy
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file contains the parser for the .def file format
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
#include <string>

#include "DefFileGroup.h"
#include "DefFileParser.h"

int yylex( void );

void yyerror( char *s ) ;

int linenum = 1;

%}

%union {
int intval;		/* for returning ints */
float floatval;	/* for returning floats */
std::string * strptr;	/* for returning strings */
}

%token <strptr> NAME
%token <strptr> STRING
%token <intval> INT
%token <floatval> FLOAT
%token <intval> LBRACE
%token <intval> RBRACE
%token <intval> EQUAL
%token <intval> SEMICOLON
%token <intval> COMMA

%type <floatval> listItem

%% /* Grammar stuff below */

input:	/* empty */
	| input group
;

group:	NAME LBRACE { DefFileGroup *tempGroup = new DefFileGroup; currentGroup->addChild( tempGroup ); currentGroup = tempGroup; currentGroup->setName( * $1 ); } groupcontents RBRACE	{  currentGroup = currentGroup->parent; delete $1; }
;

groupcontents:	/* empty */
	| groupitem
	| groupcontents groupitem
;

groupitem:	assignment
	| group
;

assignment:	NAME EQUAL NAME SEMICOLON	{ DefFileAttrib *attr = new DefFileAttrib; attr->setName( * $1 ); attr->setString( * $3 ); currentGroup->addAttribute( attr ); delete $1; delete $3; }
	| NAME EQUAL STRING SEMICOLON	{ DefFileAttrib *attr = new DefFileAttrib; attr->setName( * $1 ); attr->setString( * $3 ); currentGroup->addAttribute( attr ); delete $1; delete $3;  }
	| NAME EQUAL INT SEMICOLON	{ DefFileAttrib *attr = new DefFileAttrib; attr->setName( * $1 ); attr->setInt( $3 ); currentGroup->addAttribute( attr ); delete $1; }
	| NAME EQUAL FLOAT SEMICOLON	{ DefFileAttrib *attr = new DefFileAttrib; attr->setName( * $1 ); attr->setFloat( $3 ); currentGroup->addAttribute( attr ); delete $1;  }
	| NAME EQUAL attrlist SEMICOLON	{ tempListAttrib->setName( * $1 ); currentGroup->addAttribute( tempListAttrib ); delete $1; tempListAttrib = new DefFileAttrib;  }
;

attrlist:	listItemAndComma listItem	{ DefFileAttrib *attr = new DefFileAttrib; attr->setFloat( $2 ); tempListAttrib->appendListItem( attr ); }
;

listItem:	INT { $$ = (float)$1; /* just turn ints into floats for lists */ }
	| FLOAT
;

listItemAndComma: listItem COMMA		{ DefFileAttrib *attr = new DefFileAttrib; attr->setFloat( $1 ); tempListAttrib->appendListItem( attr ); }
	| listItemAndComma listItem COMMA	{ DefFileAttrib *attr = new DefFileAttrib; attr->setFloat( $2 ); tempListAttrib->appendListItem( attr ); }
;


%%

void yyerror( char *s ) {

	printf( "Error while reading a .def file\n" );
	printf( "Error message: %s \n", s );
	printf( "Error occurred on line %i\n", linenum );
}
