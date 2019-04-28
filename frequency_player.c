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

void sound(float freq, long duration)
{
    struct timespec ts1;
    struct timespec ts0;
    uint32_t pause;

    timespec_get(&ts0, TIME_UTC);
    timespec_get(&ts1, TIME_UTC);
    //printf("%lu\n", ts1.tv_sec*1000000000+ts1.tv_nsec);
    //printf("%lu\n", nanosecs(ts0));
    //
    pause = 0.5f / freq * 1010000.0f - 79.0;		// 109µs overhead to call usleep
    //printf("pause: %u\n", pause);

    while (nanosecs(ts1)-nanosecs(ts0) < 1000000*duration)
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
}

void play_note(float freq)
{
        bcm2835_gpio_write(PIN_05, HIGH);
        sound(freq, 250);
        bcm2835_gpio_write(PIN_05, LOW);
	usleep(100000);
}

int main(int argc, char *argv[])
{
    if(!bcm2835_init())
        return 1;

    // Set the pin to be an output
    bcm2835_gpio_fsel(PIN_11, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_05, BCM2835_GPIO_FSEL_OUTP);

    //uint32_t frequency = (rand()%13+1) * 400;    
    uint32_t frequency = 400;
    while(1)
    {  
	//for (uint32_t i=0; i<100; i++)
        //sound(frequency, 20000);
	play_note(130.813);
	play_note(261.626);
	play_note(523.251);
	play_note(1046.50);
	play_note(2093.00);
	play_note(2793.83);

	frequency += 100;
	if (frequency > 2400)
            frequency = 400;
    }

    return 0;
}
