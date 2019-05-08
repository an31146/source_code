/*
 * gcc -Wall -O2 -std=c99 -o perlin_noise_test perlin_noise_test.c perlin_noise.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double noise1(double arg);

int main(int argc, char *argv[])
{
	float _time;

	for (int i=0; i<1000000; i++)
	{
		_time = (float)clock() / CLOCKS_PER_SEC * 30.0;;
		printf("%10.6f", noise1(_time));
	}

	return 0;
}

