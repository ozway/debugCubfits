// testing values for the stable expoenentiation
#include <stdlib.h>	//malloc
#include <stdio.h>	//printf
#include <math.h>	//exp
#include <float.h>	//DBL_MAX

//void e_step_with_stable_exp(int *K, double *a_Z_normalized, double *total_sum, double *scale_exp, int *flag_out_range){
void main(){
	int k, K=5;
	double tmp_exp, max_exp;
	double* a_Z_normalized = malloc(K*sizeof(double));

//Ecoli Cfgchange value that broke the code
	// = 1459461302749408768.000000
//Larger Ecoli Cfgchange value for the same gene, same amino acid, that didn't break the code
	// = 3466220594029845504.000000

	a_Z_normalized[0]= 1459461302749408768.000000;
	a_Z_normalized[1]= 1459461302749408640.000000;
	a_Z_normalized[2]= 1459461302749408639.000000;
	a_Z_normalized[3]= 1459461302749408638.000000;
       	a_Z_normalized[4]= 0;


	double total_sum = 0.0;
	double scale_exp = 0.0;
	int flag_out_range = 0;
	max_exp = a_Z_normalized[0];
	for(k = 1; k < K; k++){
	printf("a_Z[%i] is %f\n",k,a_Z_normalized[k]);
		if(a_Z_normalized[k] > max_exp){
			max_exp = a_Z_normalized[k];
		}
	}

	/* tmp_exp = HUGE_VAL for overflow and 0 for underflow.
	 *   e.g. max_exp is large when parameters are near the boundary and is tiny when too many products.
	 */
printf("%f is max_exp\n",max_exp);

	tmp_exp = exp(max_exp);
	if(tmp_exp == HUGE_VAL || tmp_exp == 0.0){

printf("%f threw HUGE_VAL\n",tmp_exp);

		flag_out_range = 1;
		scale_exp = (tmp_exp == HUGE_VAL) ? max_exp : -max_exp;
 /*
		do{
			*scale_exp *= 0.5;
			tmp_exp = exp(*scale_exp);
		} while(tmp_exp == HUGE_VAL);
		*scale_exp = max_exp - *scale_exp;
 */


// // Logan and Cedric solution that avoids the scaling loop

//		scale_exp = max_exp - log(DBL_MAX) + 100;
//		scale_exp = max_exp - scale_exp + 10;
//printf("subtracted from ln(DBL_MAX) %f, without buffer, is %f\n", log(DBL_MAX), scale_exp);

		scale_exp = max_exp;

//Subtract from each element the distance from the highest value to the cap, plus a small additional value to push the sum below DBL_MAX



	}
	if(flag_out_range){
		for(k = 0; k < K; k++){
printf("a_Z[%i] is %f\n",k,a_Z_normalized[k]);
			a_Z_normalized[k] -= (scale_exp);
//printf("scaled a_Z[%i] is %f\n",k,a_Z_normalized[k]);
		}
	}

	total_sum = 0.0;
	for(k = 0; k < K; k++){
		a_Z_normalized[k] = exp(a_Z_normalized[k]);
//printf("exponented is %f\n", a_Z_normalized[k]);
		total_sum += a_Z_normalized[k];
//printf("total_sum is %f\n",total_sum);
	}
	for(k = 0; k < K; k++){
		a_Z_normalized[k] = a_Z_normalized[k] / total_sum;
//printf("normalized a_Z[%i] is %f\n",k,a_Z_normalized[k]);
	}
} /* End of e_step_with_stable_exp(); */
