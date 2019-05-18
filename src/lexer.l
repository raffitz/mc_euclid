
%{

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "expressions.h"
#include "conditions.h"
#include "parser.tab.h"

char* mce_solid_name;
char* mce_solid_author;
char* mce_solid_description;

int line_counter = 1;

%}

%option nounput
%option noinput

%%

"%name: "[^\n]*$	{ mce_solid_name = strdup(strchr(yytext,':')+2); }
"%author: "[^\n]*$	{ mce_solid_author = strdup(strchr(yytext,':')+2); }
"%description: "[^\n]*$	{ mce_solid_description = strdup(strchr(yytext,':')+2); }
"width: "	{ return MCE_TOKEN_SETWIDTH; }
"height: "	{ return MCE_TOKEN_SETHEIGHT; }
"depth: "	{ return MCE_TOKEN_SETDEPTH; }
"min_x: "	{ return MCE_TOKEN_SETMINX; }
"min_y: "	{ return MCE_TOKEN_SETMINY; }
"min_z: "	{ return MCE_TOKEN_SETMINZ; }
"\\"\n	{ line_counter++; }
^#[^\n]*$	{}
"\n"	{ line_counter++; return MCE_TOKEN_EOL; }
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
"sin"	{ yylval.fun = MCE_UNA_SIN; return MCE_TOKEN_FUN; }
"cos"	{ yylval.fun = MCE_UNA_COS; return MCE_TOKEN_FUN; }
"tan"	{ yylval.fun = MCE_UNA_TAN; return MCE_TOKEN_FUN; }
"sqrt"	{ yylval.fun = MCE_UNA_SQRT; return MCE_TOKEN_FUN; }
"cbrt"	{ yylval.fun = MCE_UNA_CBRT; return MCE_TOKEN_FUN; }
"pi"	{ yylval.num = M_PI; return MCE_TOKEN_NUMBER; }
"tau"	{ yylval.num = M_PI * 2; return MCE_TOKEN_NUMBER; }
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
