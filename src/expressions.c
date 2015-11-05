#include "expressions.h"

double evaluate(union expression *expression,int*vars){
	unsigned char i;
	double accum;
	struct m_sum *asum;
	struct m_product *aprod;
	struct m_var *avar;
	struct m_const *aconst;
	struct m_power *apower;
	
	asum = (struct m_sum*) expression;
	
	switch((*asum).type){
		case 's':
			accum = 0;
			for(i=0;i<(*asum).length;i++){
				accum += evaluate((union expression *) &((*asum).sumvals[i]),vars);
			}
			return accum;
			break;
		case 'p':
			aprod = (struct m_product*) asum;
			accum = 1.0;
			for(i=0;i<(*aprod).length;i++){
				accum *= evaluate((union expression *) &((*aprod).prodvals[i]),vars);
			}
			return accum;
			break;
		case 'v':
			avar = (struct m_var*) asum;
			return (*avar).coeff * vars[(*avar).index];
		case 'c':
			aconst = (struct m_const*) asum;
			return (*aconst).value;
		case 'e':
			apower = (struct m_power*) asum;
			return pow(
				evaluate((union expression *) (*apower).base,vars),
				evaluate((union expression *) (*apower).exponent,vars)
				);
		default:
			break;
	}
	
	
	return 0;
}
