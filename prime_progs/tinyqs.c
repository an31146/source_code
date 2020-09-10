/*
 * gcc.exe -Wall -O2 -std=c99 -g -I ..\..\Projects\msieve -o tinyqs.exe tinyqs.c ..\..\Projects\msieve\libmsieve.a
*/
#include <stdio.h>
#include "mp.h"

uint32 tinyqs(mp_t *n, mp_t *factor1, mp_t *factor2);

/*----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    mp_t n, p, q;
	char buffer[50];
    
    if (argc == 2)
    {
        mp_str2mp(argv[1], &n, 10);
        if (tinyqs(&n, &p, &q) != 0) 
        {
			printf("Found factors: ");
			mp_printf(&p, 10, buffer);
			printf(", ");
			mp_printf(&q, 10, buffer);
            printf("\n");
        }                
    }
    return 0;
}
