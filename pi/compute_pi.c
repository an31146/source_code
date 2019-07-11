#include <stdio.h>
#include <math.h>
#define STEPS 100000

double wallis_product(int n)
{
    int i;
    double p = 2.0;
    for (i=1; i<n; i++)
    {
        //p*=((double)(((int)((i+2)/2))*2))/(((int)((i+1)/2))*2+1);
        p *= (double)(i*2)/(i*2-1);
        p *= (double)(i*2)/(i*2+1);
        //printf("%10d\r",i);
    }
    return p;
}

double archimedes(int n)
{
	// Calculate n iterations of Archimedes PI recurrence relation 
    double squared = 2.0;
	unsigned long sides = 4;
	int i;
	
	for (i=0; i<n; i++)
	{
		squared = 2.0 - 2.0*sqrt(1.0 - squared/4.0);
		//printf("%.20f\n", squared);
		sides <<= 1;
	}
	return (double)sides * sqrt(squared)/2.0;
}

double integral_x_squared(int steps)
{
    double sum, x = 0, inc;
    int i;
    sum = 0;
    inc = 1.0/steps;
    
    for (i=0; i<steps; i++, x+=inc)
        sum += 1.0 / (1.0 + x*x);
        
    return 4.0*sum/steps;
}

double bbp_formula(int steps)
{
    double sum = 0, t;
    int k;
    
    for (k=0; k<steps; k++)
    {
        t = 4.0/(8*k+1) - 2.0/(8*k+4) - 1.0/(8*k+5) - 1.0/(8*k+6);
        //printf("%.20f\n", t * pow(16.0d, (double)-k));
        sum += t / pow(16.0, (float)k);
    }
    return sum;
}

double factorial(int n)
{
	double i;
	
	if (n <= 1)
		return 1.0;
	for (i=n; n-->1;)
	{
		i *= (double)n;
		//printf("%.0f\n", i);
	}
	return i;
}

double taylor_series(int steps)
{
	double sum = 2.0;
	int k;
	
	for (k=2; k<steps; k++)
	{
		sum += 1.0 / factorial(k);
	}
	return sum;
}

double zeta2(int steps)
{
	double sum = 1.0;
	int k;
	
	for (k=2; k<steps; k++)
	{
		sum += 1.0 / ((double)k * k);
	}
	return sqrt(sum * 6.0);
}

double zeta4(int steps)
{
	double sum = 1.0;
	int k;
	
	for (k=2; k<steps; k++)
	{
		sum += pow(k, -4.0);
	}
	return pow(sum * 90.0, 0.25);
}

double Ramanujan()
{
	return (log(pow(640320.0, 3.0)+744) / sqrt(163));
}

double sum_one_over_m_to_power_n(double m, int steps)
{
    double sum = 1.0;
    int k;
    
    for (k=1; k<steps; k++)
    {
        sum += pow(m, -1.0 * k);
    }
    return sum;
}

double sum__minus_one_to_the_n_over_two_to_the_n(double m, int steps)
{
    double sum = 1.0;
    int k;
    
    for (k=1; k<steps; k++)
    {
        sum += pow(-1.0, k) * pow(m, -1.0 * k);
    }
    return sum;
}

int main()
{
    double m = 5.0;
    printf("integral_x_squared\npi = %.20f\n\n", integral_x_squared(STEPS));
    printf("Wallis_product\npi = %.20f\n\n", wallis_product(STEPS));
    printf("Archimedes\npi = %.20f\n\n", archimedes(15));
    printf("bbp_formula\npi = %.20f\n\n", bbp_formula(11));
    printf("zeta2\npi = %.20f\n\n", zeta2(STEPS));
    printf("zeta4\npi = %.20f\n\n", zeta4(STEPS));
    printf("Ramanujan\npi = %.20f\n\n", Ramanujan());
    printf("factorial(20) = %.0f\n\n", factorial(20));
    printf("exp(1) = %.18f\n\n", taylor_series(18));
    printf("sum(1/%.1f^n) = %.18f\n\n", m, sum_one_over_m_to_power_n(m, STEPS));
    printf("sum(-1^n/%.1f^n) = %.18f\n\n", m, sum__minus_one_to_the_n_over_two_to_the_n(m, STEPS));
    //printf("%.20f\n",sqrt(2));
    getchar();
    return 0;
}