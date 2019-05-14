#ifndef MCE_HEADER_CONDITIONS
#define MCE_HEADER_CONDITIONS

#include "expressions.h"
#include <stdint.h>

enum mce_conditions_type{
	MCE_CONDITION_LT,
	MCE_CONDITION_GT,
	MCE_CONDITION_LTE,
	MCE_CONDITION_GTE
};

struct mce_condition{
	enum mce_conditions_type type;
	union mce_expression* left;
	union mce_expression* right;
	struct mce_condition* prev;
	struct mce_condition* next;
};

void mce_condition(enum mce_conditions_type type, union mce_expression* left, union mce_expression* right, struct mce_condition** first, struct mce_condition** last);
void mce_condition_at_start(enum mce_conditions_type type, union mce_expression* left, union mce_expression* right, struct mce_condition** first, struct mce_condition** last);
void mce_condition_at_end(enum mce_conditions_type type, union mce_expression* left, union mce_expression* right, struct mce_condition** first, struct mce_condition** last);

uint8_t mce_check(struct mce_condition* condition, struct mce_vars vars);

void mce_free_conditions(struct mce_condition** first, struct mce_condition** last);

#endif
