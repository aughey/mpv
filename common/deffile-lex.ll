%option noyywrap
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
 *  FILENAME:   deffile-lex.ll
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file contains the lexical analyzer for the .def file format
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

#include <math.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cctype>
#include "deffile-yacc.hpp"

#ifdef WIN32
#define snprintf _snprintf
#define isatty(x) 0
#endif

extern int linenum;

void yyerror( char *s ) ;
%}

ID	[a-zA-Z0-9][_:/a-zA-Z0-9]*
/*HEX	-?"0x"?[0-9A-F]+*/
/*{HEX}	{ yylval.intval = strtol( yytext, NULL, 16 ); return INT; }*/

/* We have an "exclusive start condition", the c-style comments */
%x comment

%%

"{"	{ return LBRACE; }
"}"	{ return RBRACE; }
"="	{ return EQUAL; }
";"	{ return SEMICOLON; }
","	{ return COMMA; }
"true"|"on"	{ yylval.intval = 1; return INT; }
"false"|"off"	{ yylval.intval = 0; return INT; }
-?[0-9]+"."[0-9]*|-?"."[0-9]+	{ yylval.floatval = atof( yytext ); return FLOAT; }
-?[0-9]*	{ yylval.intval = atoi( yytext ); return INT; }
{ID}	{ std::string *str = yylval.strptr = new std::string( yytext ); std::transform( str->begin(), str->end(), str->begin(), (int(*)(int))tolower ); return NAME; }
\".*\"	{ yylval.strptr = new std::string( yytext ); yylval.strptr->erase( yylval.strptr->begin() ); yylval.strptr->erase( --yylval.strptr->end() ); return STRING; }
[ \t\r]+	{ ; /* whitespace */ }
\n			{ linenum++; /* new line */ }
"//".*\n	{ linenum++; /* c++ style comments */ }
"/*"		BEGIN(comment); /* switch to the "comment" state */

<comment>[^*\n]*		/* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*	/* eat up '*'s not followed by '/'s */
<comment>\n 			{ linenum++; /* keep the line number up-to-date */ }
<comment>"*"+"/"		BEGIN(INITIAL); /* switch back to the "initial" state */

.	{ { char tempstring[256]; snprintf( tempstring, 255, "illegal token: %s", yytext ); yyerror(tempstring); } }

%%

void flushDefLexer()
{
	YY_FLUSH_BUFFER;
}

/*
int main( void ) {
	
	yylex();
	printf( "that's it\n" );
	
	
	return 0;
}
*/
