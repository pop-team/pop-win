%{
	#include <stdio.h>
	#include "y.tab.h"
	#include <stdlib.h>     /* atoi */
	extern YYSTYPE yylval;
%}

id ["*"]*[_a-zA-Z][_a-zA-Z0-9]*["*"]*
temp template
pac pack
uid classuid
digit [0-9]+
include "#"[\t]*"include"
str (\"([^\"\r\n]|"\\\"")*\") 

vct [\t]*"vector"

operator	["::""."]+
symbol		["("")"";"":""*""=""@""\[""\]""&""/""!""\'"]

lbrace		["{"]
rbrace		["}"]

spkeyword	["~"]

%%

[ \a\b\v\f\t\r]+ {}
{include}	{return INCLUDE;}
{temp} 	{return TEMP;}
{vct}	{return VEC;}
{digit}{operator}{digit}	{yylval.str = strdup(yytext); return DECPOINT;}

{pac}	{yylval.str = strdup(yytext); return PACK;}

[","]	{yylval.str = strdup(yytext); return COMMA;}

{uid}{symbol}	{yylval.str = strdup(yytext); return UID;}

{id} 	{yylval.str = strdup(yytext); return ID;}
\n 	{return END;}


["\<""\>"]	{yylval.str = strdup(yytext);return ALR;}
{digit}	{yylval.num = atoi(yytext); return DIGIT;}



{str}	{yylval.str = strdup(yytext); return INSTR;}

#	{return WELL;}


\<\<	{return DAL;}
\>\>	{return DAR;}

{id}{operator}{id} {yylval.str = strdup(yytext); return OPERATOR;}

{symbol} {yylval.str = strdup(yytext); return SYMBOL;}

{lbrace} {yylval.str = strdup(yytext); return LBRACE;}
{rbrace} {yylval.str = strdup(yytext); return RBRACE;}

{spkeyword}{id}	{yylval.str = strdup(yytext); return SPKEYWORD;}

%%
	int yywrap(void) {
	return 1;
	}
