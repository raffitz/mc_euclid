#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "expressions.h"

union mce_expression* mce_new(){
	void* pointer = malloc(sizeof(union mce_expression));
	if (pointer == NULL){
		perror("mce_new");
		exit(-1);
	}
	return (union mce_expression*) pointer;
}

union mce_expression* mce_val(double value){
	union mce_expression* pointer = mce_new();

	(*pointer).id.type = MCE_EXP_VAL;
	(*pointer).val.value = value;

	return pointer;
}

union mce_expression* mce_var(enum mce_variable var){
	union mce_expression* pointer = mce_new();

	(*pointer).id.type = MCE_EXP_VAR;
	(*pointer).var.var = var;

	return pointer;
}

union mce_expression* mce_binary(enum mce_binary_type op, union mce_expression* left, union mce_expression* right){
	union mce_expression* pointer = mce_new();

	(*pointer).id.type = MCE_EXP_BIN;
	(*pointer).binary.operation = op;
	(*pointer).binary.left = left;
	(*pointer).binary.right = right;

	return pointer;
}

union mce_expression* mce_pow(union mce_expression* base, union mce_expression* power){
	return mce_binary(MCE_BIN_POW,base,power);
}

union mce_expression* mce_mul(union mce_expression* left, union mce_expression* right){
	return mce_binary(MCE_BIN_MUL, left, right);
}

union mce_expression* mce_div(union mce_expression* dividend, union mce_expression* divisor){
	return mce_binary(MCE_BIN_DIV, dividend, divisor);
}

union mce_expression* mce_add(union mce_expression* left, union mce_expression* right){
	return mce_binary(MCE_BIN_ADD, left, right);
}

union mce_expression* mce_sub(union mce_expression* minuend, union mce_expression* subtrahend){
	return mce_binary(MCE_BIN_SUB, minuend, subtrahend);
}

union mce_expression* mce_unary(enum mce_unary_type fn, union mce_expression* argument){
	union mce_expression* pointer = mce_new();

	(*pointer).id.type = MCE_EXP_UNA;
	(*pointer).unary.function = fn;
	(*pointer).unary.argument = argument;

	return pointer;
}

double mce_resolve_super(union mce_expression* root, struct mce_vars vars, uint8_t allow_xyz){
	double cartesian_vars;

	switch((*root).id.type){
		case MCE_EXP_VAL:
			return (*root).val.value;
		case MCE_EXP_VAR:
			switch((*root).var.var){
				case MCE_VAR_S:
					return vars.s;
				case MCE_VAR_X:
					cartesian_vars = vars.x;
					break;
				case MCE_VAR_Y:
					cartesian_vars = vars.y;
					break;
				case MCE_VAR_Z:
					cartesian_vars = vars.z;
					break;
				default:
					fprintf(stderr,"Unrecognised variable\n");
					exit(-1);
			}
			if (allow_xyz == 0){
				fprintf(stderr,"Illegal usage of coordinates in boundary definitions\n");
				exit(-1);
			}
			return cartesian_vars;
		case MCE_EXP_BIN:
			switch((*root).binary.operation){
				case MCE_BIN_POW:
					return pow(mce_resolve_super((*root).binary.left, vars, allow_xyz), mce_resolve_super((*root).binary.right, vars, allow_xyz));
				case MCE_BIN_MUL:
					return mce_resolve_super((*root).binary.left, vars, allow_xyz) * mce_resolve_super((*root).binary.right, vars, allow_xyz);
				case MCE_BIN_DIV:
					return mce_resolve_super((*root).binary.left, vars, allow_xyz) / mce_resolve_super((*root).binary.right, vars, allow_xyz);
				case MCE_BIN_ADD:
					return mce_resolve_super((*root).binary.left, vars, allow_xyz) + mce_resolve_super((*root).binary.right, vars, allow_xyz);
				case MCE_BIN_SUB:
					return mce_resolve_super((*root).binary.left, vars, allow_xyz) - mce_resolve_super((*root).binary.right, vars, allow_xyz);
				default:
					fprintf(stderr,"Unrecognised binary operation\n");
					exit(-1);
			}
		case MCE_EXP_UNA:
			switch((*root).unary.function){
				case MCE_UNA_SIN:
					return sin(mce_resolve_super((*root).unary.argument,vars,allow_xyz));
				case MCE_UNA_COS:
					return cos(mce_resolve_super((*root).unary.argument,vars,allow_xyz));
				case MCE_UNA_TAN:
					return tan(mce_resolve_super((*root).unary.argument,vars,allow_xyz));
				case MCE_UNA_SQRT:
					return sqrt(mce_resolve_super((*root).unary.argument,vars,allow_xyz));
				case MCE_UNA_CBRT:
					return cbrt(mce_resolve_super((*root).unary.argument,vars,allow_xyz));
				default:
					fprintf(stderr,"Unrecognised unary operation\n");
					exit(-1);
			}
		default:
			fprintf(stderr,"Unrecognised token value");
			exit(-1);
	}
}

double mce_resolve_def(union mce_expression* root, struct mce_vars vars){
	return mce_resolve_super(root, vars, 0);
}

double mce_resolve(union mce_expression* root, struct mce_vars vars){
	return mce_resolve_super(root, vars, 1);
}
