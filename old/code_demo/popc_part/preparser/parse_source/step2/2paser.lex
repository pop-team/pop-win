%{
	#include <stdio.h>
	#include "y.tab.h"
	#include <stdlib.h>     /* atoi */
	extern YYSTYPE yylval;
%}

id ["*"]*[_a-zA-Z][_a-zA-Z0-9]*["*"]*
temp template
digit [0-9]+[\.0-9]*
include "#"[\t]*"include"
str (\"([^\"\r\n]|"\\\"")*\") 

vct [\t]*"vector"

operator	["::""."]+
symbol		["("")"";"":""*""=""\[""\]""&""/""!""\'"]
spkeyword	["~"]


%%

[ \a\b\v\f\t\r]+ {}
{temp} 	{return TEMP;}
{vct}	{return VEC;}
{digit}{operator}{digit}	{yylval.str = strdup(yytext); return DECPOINT;}

{id} 	{yylval.str = strdup(yytext); return ID;}
\n 	{return END;}
["\<""\>"]	{yylval.str = strdup(yytext);return ALR;}
		
{digit}	{yylval.num = atoi(yytext); return DIGIT;}
{include}	{return INCLUDE;}

[","]	{yylval.str = strdup(yytext); return COMMA;}
{str}	{yylval.str = strdup(yytext); return INSTR;}

#	{return WELL;}



\<\<	{return DAL;}
\>\>	{return DAR;}

{id}{operator}{id} {yylval.str = strdup(yytext); return OPERATOR;}

{symbol} {yylval.str = strdup(yytext); return SYMBOL;}

{spkeyword}{id}	{yylval.str = strdup(yytext); return SPKEYWORD;}
%%
	int yywrap(void) {
	return 1;
	}
