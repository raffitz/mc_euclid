
%{

#include <string.h>
#include <stdio.h>
#include "expressions.h"
#include "conditions.h"
#include "parser.tab.h"

char* mce_solid_name;
char* mce_solid_author;
char* mce_solid_description;

%}

%%

"%name: "[^\n]*$	{ mce_solid_name = strdup(strchr(yytext,':')+2); return MCE_TOKEN_EOL; }
"%author: "[^\n]*$	{ mce_solid_author = strdup(strchr(yytext,':')+2); return MCE_TOKEN_EOL; }
"%description: "[^\n]*$	{ mce_solid_description = strdup(strchr(yytext,':')+2); return MCE_TOKEN_EOL; }
"width: "	{ return MCE_TOKEN_SETWIDTH; }
"height: "	{ return MCE_TOKEN_SETHEIGHT; }
"depth: "	{ return MCE_TOKEN_SETDEPTH; }
"min_x: "	{ return MCE_TOKEN_SETMINX; }
"min_y: "	{ return MCE_TOKEN_SETMINY; }
"min_z: "	{ return MCE_TOKEN_SETMINZ; }
^#[^\n]*$	{ return MCE_TOKEN_EOL; }
"\n"	{ return MCE_TOKEN_EOL; }
-?[0-9]+\.?[0-9]*	{
		double aux;

		sscanf(yytext,"%lf",&aux);
		yylval.num = aux;
		return MCE_TOKEN_NUMBER;
	}
"s"	{ yylval.var = MCE_VAR_S; return MCE_TOKEN_VARIABLE; }
"x"	{ yylval.var = MCE_VAR_X; return MCE_TOKEN_VARIABLE; }
"y"	{ yylval.var = MCE_VAR_Y; return MCE_TOKEN_VARIABLE; }
"z"	{ yylval.var = MCE_VAR_Z; return MCE_TOKEN_VARIABLE; }
"("	{ return MCE_TOKEN_LPAREN; }
")"	{ return MCE_TOKEN_RPAREN; }
"^"	{ return MCE_TOKEN_POW; }
"*"	{ return MCE_TOKEN_MUL; }
"/"	{ return MCE_TOKEN_DIV; }
"+"	{ return MCE_TOKEN_ADD; }
"-"	{ return MCE_TOKEN_SUB; }
"<"	{ yylval.cond = MCE_CONDITION_LT; return MCE_TOKEN_CONDITION; }
">"	{ yylval.cond = MCE_CONDITION_GT; return MCE_TOKEN_CONDITION; }
"<="	{ yylval.cond = MCE_CONDITION_LTE; return MCE_TOKEN_CONDITION; }
">="	{ yylval.cond = MCE_CONDITION_GTE; return MCE_TOKEN_CONDITION; }
[ \t]+	{}
.	{ fprintf(stderr,"%s",yytext); return MCE_TOKEN_EOL; }

%%