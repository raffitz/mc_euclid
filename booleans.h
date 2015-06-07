#include <stdint.h>
#include "expressions.h"

#ifndef MCE_BOOLEANS
#define MCE_BOOLEANS

struct b_and{
	char type;
	unsigned char length;
	void** ands;
};

struct b_or{
	char type;
	unsigned char length;
	void** ors;
};

struct b_ltoez{
	char type;
	unsigned char index;
};

union boolean{
	struct b_and band;
	struct b_or bor;
	struct b_ltoez bltoez;
};

uint8_t bevaluate(union boolean* boolean, union expression* expressions, int* vars);

#endif
