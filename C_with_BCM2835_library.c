/*
 *  gcc -Wall -O2 -std=gnu11 -o C_with_BCM2835_library C_with_BCM2835_library.c -lbcm2835
 */
#define __USE_POSIX199309

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h>

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <time.h>

#define PIN_11 RPI_V2_GPIO_P1_23  // GPIO 11
#define PIN_05 RPI_V2_GPIO_P1_29  // GPIO 11

#define UNUSED __attribute__((unused))

#define nanosecs(TS) ((TS.tv_sec*1000000000+TS.tv_nsec))


int main(int argc, char *argv[])
{
    if(!bcm2835_init())
        return 1;

    // Set the pin to be an output
    bcm2835_gpio_fsel(PIN_11, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_05, BCM2835_GPIO_FSEL_OUTP);

    uint32_t pause = (rand()%13+1) * 40;    
    while(1)
    {  
        bcm2835_gpio_write(PIN_05, HIGH);

        //uint32_t pause = (rand()%13+1) * 40;    
	//for (uint32_t i=0; i<100; i++)
	struct timespec ts1;
       	struct timespec ts0;

	timespec_get(&ts0, TIME_UTC);
	timespec_get(&ts1, TIME_UTC);
	//printf("%lu\n", ts1.tv_sec*1000000000+ts1.tv_nsec);
	//printf("%lu\n", nanosecs(ts0));

	while (nanosecs(ts1)-nanosecs(ts0) < 100000000)
	{
            bcm2835_gpio_write(PIN_11, HIGH);
            //delay(1);
	    usleep(pause);

            bcm2835_gpio_write(PIN_11, LOW);
            //delay(1);
	    usleep(pause);

	    timespec_get(&ts1, TIME_UTC);
	    //printf("%ld\n", nanosecs(ts1)-nanosecs(ts0));
	}
        bcm2835_gpio_write(PIN_05, LOW);
	usleep(250000);

	pause += 20;
	if (pause > 1200)
            pause = 20;
    }

    return 0;
}
