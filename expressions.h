
#ifndef MCE_EXPRESSIONS
#define MCE_EXPRESSIONS

struct m_sum{
	char type;
	unsigned char length;
	void** sumvals;
};
struct m_product{
	char type;
	unsigned char length;
	void** prodvals;
};
struct m_const{
	char type;
	double value;
};
struct m_var{
	char type;
	unsigned char index;
};

union expression{
	struct m_sum sum;
	struct m_product product;
	struct m_const constant;
	struct m_var var;
};

double evaluate(union expression *expression,int*vars);

#endif
