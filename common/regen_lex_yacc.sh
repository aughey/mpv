#!/bin/sh
bison -d -o deffile-yacc.cpp deffile-yacc.yy
flex -o deffile-lex.cpp deffile-lex.ll
