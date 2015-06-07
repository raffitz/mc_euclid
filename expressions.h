#include <math.h>

#ifndef MCE_EXPRESSIONS
#define MCE_EXPRESSIONS

typedef union expression exprssn;

struct m_sum{
	char type;
	int length;
	union expression* sumvals;
};
struct m_product{
	char type;
	int length;
	union expression* prodvals;
};
struct m_const{
	char type;
	double value;
};
struct m_var{
	char type;
	unsigned char index;
	double coeff;
};
struct m_power{
	char type;
	union expression* exponent;
	union expression* base;
};

union expression{
	struct m_sum sum;
	struct m_product product;
	struct m_const constant;
	struct m_var var;
	struct m_power power;
};

double evaluate(union expression *expression,int*vars);

#endif
