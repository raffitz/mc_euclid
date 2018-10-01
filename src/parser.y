%{
#include "expressions.h"
#include "conditions.h"
#include <stddef.h>
#include <stdint.h>

int yylex();
int yyerror(char*);

double width, height, depth;
double minx, miny, minz;
uint8_t checks = 0;

struct mce_condition* first = NULL;
struct mce_condition* last = NULL;

extern struct mce_vars vars;

%}

%union {
	double num;
	enum mce_variable var;
	union mce_expression* exp;
	enum mce_conditions_type cond;
}

%token MCE_TOKEN_SETWIDTH
%token MCE_TOKEN_SETHEIGHT
%token MCE_TOKEN_SETDEPTH
%token MCE_TOKEN_SETMINX
%token MCE_TOKEN_SETMINY
%token MCE_TOKEN_SETMINZ

%token MCE_TOKEN_EOL
%token <num> MCE_TOKEN_NUMBER
%token <var> MCE_TOKEN_VARIABLE
%token MCE_TOKEN_LPAREN
%token MCE_TOKEN_RPAREN

%right <cond> MCE_TOKEN_CONDITION

%left MCE_TOKEN_ADD
%left MCE_TOKEN_SUB
%left MCE_TOKEN_MUL
%left MCE_TOKEN_DIV
%left MCE_TOKEN_POW

%type <exp> expression

%start lines

%%
expression:	expression MCE_TOKEN_ADD expression	{ $$ = mce_add($1,$3); }
	|	expression MCE_TOKEN_SUB expression	{ $$ = mce_sub($1,$3); }
	|	expression MCE_TOKEN_MUL expression	{ $$ = mce_mul($1,$3); }
	|	expression MCE_TOKEN_DIV expression	{ $$ = mce_div($1,$3); }
	|	expression MCE_TOKEN_POW expression	{ $$ = mce_pow($1,$3); }
	|	MCE_TOKEN_NUMBER	{ $$ = mce_val($1); }
	|	MCE_TOKEN_VARIABLE	{ $$ = mce_var($1); }
	|	MCE_TOKEN_LPAREN expression MCE_TOKEN_RPAREN	{ $$ = $2; }
	;

definition:	MCE_TOKEN_SETWIDTH expression	{ width = mce_resolve_def($2, vars); checks = checks | 1; }
	|	MCE_TOKEN_SETHEIGHT expression	{ height = mce_resolve_def($2, vars); checks = checks | 2; }
	|	MCE_TOKEN_SETDEPTH expression	{ depth = mce_resolve_def($2, vars); checks = checks | 4; }
	|	MCE_TOKEN_SETMINX expression	{ minx = mce_resolve_def($2, vars); checks = checks | 8; }
	|	MCE_TOKEN_SETMINY expression	{ miny = mce_resolve_def($2, vars); checks = checks | 16; }
	|	MCE_TOKEN_SETMINZ expression	{ minz = mce_resolve_def($2, vars); checks = checks | 32; }
	;

condition:	expression MCE_TOKEN_CONDITION expression	{ mce_condition_at_end($2, $1, $3, &first, &last); };

lines:
	|	lines MCE_TOKEN_EOL	{}
	|	lines definition MCE_TOKEN_EOL	{}
	|	lines condition MCE_TOKEN_EOL	{}
	;

%%
