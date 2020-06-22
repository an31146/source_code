/*Arithmetic Geometric Mean of 1 and 1/sqrt(2)
 
  Nigel_Galloway
  February 7th., 2012.
*/
 
#include <gmp.h>
 
void agm (const mpf_t in1, const mpf_t in2, mpf_t out1, mpf_t out2) {
	mpf_add (out1, in1, in2);
	mpf_div_ui (out1, out1, 2);
	mpf_mul (out2, in1, in2);
	mpf_sqrt (out2, out2);
}
 
int main (void) {
	mpf_set_default_prec (2048);
	mpf_t x0, y0, resA, resB;
 
	mpf_init_set_ui (y0, 1);
	mpf_init_set_d (x0, 2);
    //mpf_mul_2exp (x0, x0, 2048);
    //mpf_ui_div (x0, 4, x0);
	mpf_sqrt (x0, x0);

    gmp_printf("x0 = %.500Ff\n\n", x0);
    gmp_printf("y0 = %.500Ff\n\n", y0);
    
	mpf_init (resA);
	mpf_init (resB);
 
	for(int i=0; i<11; i++){
		agm(x0, y0, resA, resB);
		agm(resA, resB, x0, y0);
	}
	gmp_printf ("%.500Ff\n\n", x0);
    mpf_ui_div (y0, 1.0, y0);
	gmp_printf ("%.500Ff\n\n", y0);
 
	return 0;
}
