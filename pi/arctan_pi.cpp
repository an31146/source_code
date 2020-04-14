/* 
 *  Calculate pi using arctan series method
 *	
 *	g++ -Wall -O2 -std=c++11 -o arctan_pi.exe arctan_pi.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double fact(int n)
{
    double f = 1.0d;
    
    if (n == 0 || n == 1)
        return 1.0d;
    for (int i = 2; i <= n; i++)
        f *= (double)i;
    return f;
}

double double_fact(int n)
{
    double f;
    
    if (n % 2 == 0)
    {
        f = 2.0d;
        for (int i = n; i > 2; i -= 2)
            f *= (double)i;
    }
    else
    {
        f = 1.0d;
        for (int i = n; i > 1; i -= 2)
            f *= (double)i;
    }
    return f;
}

int main(void)
{
    double pi, sum = 0.0d;

    for (int j = 0; j < 6; j++)
    {
        for (int i = j*10+1; i <= j*10+10; i++)
        {
            //printf("%d\n", i);
            sum += fact(i) / double_fact(2*i+1);
        }
        
        pi = 2.0d + 2.0d * sum;
        printf("%.18f\n", pi);
    }
    
    return EXIT_SUCCESS;
}