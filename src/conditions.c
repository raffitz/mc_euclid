#include "conditions.h"
#include <stdlib.h>
#include <stdio.h>

void mce_condition(enum mce_conditions_type type, union mce_expression* left, union mce_expression* right, struct mce_condition** first, struct mce_condition** last){
	mce_condition_at_end(type, left, right, first, last);
}

void mce_condition_at_start(enum mce_conditions_type type, union mce_expression* left, union mce_expression* right, struct mce_condition** first, struct mce_condition** last){
	struct mce_condition* aux = (struct mce_condition*) malloc(sizeof(struct mce_condition));

	(*aux).type = type;
	(*aux).left = left;
	(*aux).right = right;
	(*aux).prev = NULL;
	(*aux).next = (*first);

	if ((*first) != NULL) (*(*first)).prev = aux;
	(*first) = aux;
	if ((*last) == NULL) (*last) = aux;
}

void mce_condition_at_end(enum mce_conditions_type type, union mce_expression* left, union mce_expression* right, struct mce_condition** first, struct mce_condition** last){
	struct mce_condition* aux = (struct mce_condition*) malloc(sizeof(struct mce_condition));

	(*aux).type = type;
	(*aux).left = left;
	(*aux).right = right;
	(*aux).prev = (*last);
	(*aux).next = NULL;

	if ((*last) != NULL) (*(*last)).next = aux;
	(*last) = aux;
	if ((*first) == NULL) (*first) = aux;
}

uint8_t mce_check(struct mce_condition* condition, struct mce_vars vars){
	double left = mce_resolve((*condition).left, vars);
	double right = mce_resolve((*condition).right, vars);

	switch((*condition).type){
		case MCE_CONDITION_LT:
			return left < right;
		case MCE_CONDITION_GT:
			return left > right;
		case MCE_CONDITION_LTE:
			return left <= right;
		case MCE_CONDITION_GTE:
			return left >= right;
		default:
			fprintf(stderr,"Unrecognised condition\n");
			exit(-1);
	}
}

void mce_print_conditions(struct mce_condition* condition){
	printf("{");
	mce_print_expressions((*condition).left);
	switch((*condition).type){
		case MCE_CONDITION_LT:
			printf("<");
			break;
		case MCE_CONDITION_GT:
			printf(">");
			break;
		case MCE_CONDITION_LTE:
			printf("<=");
			break;
		case MCE_CONDITION_GTE:
			printf(">=");
			break;
		default:
			printf("??");
			break;
	}
	mce_print_expressions((*condition).right);
	printf("};\n");
}

void mce_free_conditions(struct mce_condition** first, struct mce_condition** last){
	struct mce_condition* aux;

	while((*first) != NULL){
		aux = (*(*first)).next;
		mce_free_expression((*(*first)).left);
		mce_free_expression((*(*first)).right);
		free(*first);
		(*first) = aux;
	}
	(*last) = NULL;
}
