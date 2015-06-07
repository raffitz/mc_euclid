#include <stdint.h>
#include "expressions.h"

#ifndef MCE_BOOLEANS
#define MCE_BOOLEANS

typedef union boolean boolean;

struct b_and{
	char type;
	int length;
	union boolean* ands;
};

struct b_or{
	char type;
	int length;
	union boolean* ors;
};

struct b_ltoez{
	char type;
	int index;
};

union boolean{
	struct b_and band;
	struct b_or bor;
	struct b_ltoez bltoez;
};

uint8_t bevaluate(union boolean* boolean, union expression* expressions, int* vars);

#endif
