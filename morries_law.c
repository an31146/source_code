#include <stdio.h>
#include <math.h>

#define PI 3.141592653589793234626433832795

int main()
{
	printf("The curious identity known as Morrie's law:\n");
	printf("  cos(π/9).cos(2π/9).cos(4π/9) = %f\n", cos(PI/9.0f) * cos(2*PI/9.0f) * cos(4*PI/9.0f));

	return 0;
}
