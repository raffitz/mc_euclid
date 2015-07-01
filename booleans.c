#include "booleans.h"

uint8_t bevaluate(union boolean* boolean, union expression* expressions, int* vars){
	unsigned char i;
	uint8_t accum;
	double aux;
	struct b_and *aand;
	struct b_or *aor;
	struct b_ltoez *altoez;

	aand = (struct b_and*) boolean;

	switch((*aand).type){
		case 'a':
			accum = 0;
			for(i=0;i<(*aand).length;i++){
				if(bevaluate((union boolean*)&((*aand).ands[i]),expressions,vars)){
					accum++;
				}
			}
#			ifdef MCE_DEBUG3
			printf("%d/%d\n",accum,(*aand).length);
#			endif
			if(accum == (*aand).length){
				return 1;
			}else return 0;
		case 'o':
			aor = (struct b_or *)aand;

			for(i=0;i<(*aor).length;i++){
				if(bevaluate((union boolean*)&((*aor).ors[i]),expressions,vars)){
					return 1;
				}
			}
			return 0;
		case 'l':
			altoez = (struct b_ltoez *)aand;
			aux = evaluate(&(expressions[(*altoez).index]),vars);
			return aux<= 0.0;
		default:
			break;
	}
	return 0;
}
