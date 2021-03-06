%{
#include "expressions.h"
#include "conditions.h"
#include <stddef.h>
#include <stdint.h>

int yylex();
int yyerror(char*);

double mce_def_width, mce_def_height, mce_def_depth;
double mce_def_min_x, mce_def_min_y, mce_def_min_z;
uint8_t mce_definition_check = 0;

struct mce_condition* first = NULL;
struct mce_condition* last = NULL;

int scale;

%}

%union {
	double num;
	enum mce_variable var;
	union mce_expression* exp;
	enum mce_conditions_type cond;
	enum mce_unary_type fun;
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

%nonassoc <fun> MCE_TOKEN_FUN

%type <exp> expression

%start lines

%%
expression:	expression MCE_TOKEN_ADD expression	{ $$ = mce_add($1,$3); }
	|	expression MCE_TOKEN_SUB expression	{ $$ = mce_sub($1,$3); }
	|	expression MCE_TOKEN_MUL expression	{ $$ = mce_mul($1,$3); }
	|	expression MCE_TOKEN_DIV expression	{ $$ = mce_div($1,$3); }
	|	expression MCE_TOKEN_POW expression	{ $$ = mce_pow($1,$3); }
	|	MCE_TOKEN_FUN expression	{ $$ = mce_unary($1,$2); }
	|	MCE_TOKEN_NUMBER	{ $$ = mce_val($1); }
	|	MCE_TOKEN_VARIABLE	{ $$ = mce_var($1); }
	|	MCE_TOKEN_LPAREN expression MCE_TOKEN_RPAREN	{ $$ = $2; }
	;

definition:	MCE_TOKEN_SETWIDTH expression	{
		mce_def_width = mce_resolve_def($2, mce_vars_def(scale));
		mce_definition_check = mce_definition_check | 1;
		mce_free_expression($2);
		}
	|	MCE_TOKEN_SETHEIGHT expression	{
		mce_def_height = mce_resolve_def($2, mce_vars_def(scale));
		mce_definition_check = mce_definition_check | 2;
		mce_free_expression($2);
		}
	|	MCE_TOKEN_SETDEPTH expression	{
		mce_def_depth = mce_resolve_def($2, mce_vars_def(scale));
		mce_definition_check = mce_definition_check | 4;
		mce_free_expression($2);
		}
	|	MCE_TOKEN_SETMINX expression	{
		mce_def_min_x = mce_resolve_def($2, mce_vars_def(scale));
		mce_definition_check = mce_definition_check | 8;
		mce_free_expression($2);
		}
	|	MCE_TOKEN_SETMINY expression	{
		mce_def_min_y = mce_resolve_def($2, mce_vars_def(scale));
		mce_definition_check = mce_definition_check | 16;
		mce_free_expression($2);
		}
	|	MCE_TOKEN_SETMINZ expression	{
		mce_def_min_z = mce_resolve_def($2, mce_vars_def(scale));
		mce_definition_check = mce_definition_check | 32;
		mce_free_expression($2);
		}
	;

condition:	expression MCE_TOKEN_CONDITION expression	{ mce_condition_at_end($2, $1, $3, &first, &last); };

lines:
	|	lines MCE_TOKEN_EOL	{}
	|	lines definition MCE_TOKEN_EOL	{}
	|	lines condition MCE_TOKEN_EOL	{}
	;

%%
