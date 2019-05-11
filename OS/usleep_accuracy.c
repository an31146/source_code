#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void nsleep(long us)
{
    struct timespec wait;
    /* printf("Will sleep for is %ld\n", diff); // This will take extra ~70 microseconds */
    
    wait.tv_sec = us / (1000 * 1000);
    wait.tv_nsec = (us % (1000 * 1000)) * 1000;
    nanosleep(&wait, NULL);
}

int main()
{
    struct timeval t1, t2;
    long long t;
	
    long microseconds = 773000;
	
    /* nanosleep test */
    gettimeofday(&t1, NULL);
    nsleep(microseconds);
    gettimeofday(&t2, NULL);
	
    t = ((t2.tv_sec * 1000000) + t2.tv_usec) - ((t1.tv_sec * 1000000) + t1.tv_usec);
    printf("Call to nsleep(%ld) took %lld\n", microseconds, t);

    /* usleep test */   
    gettimeofday(&t1, NULL);
    usleep(microseconds);
    gettimeofday(&t2, NULL);
	
    t = ((t2.tv_sec * 1000000) + t2.tv_usec) - ((t1.tv_sec * 1000000) + t1.tv_usec);
    printf("Call to usleep(%ld) took %lld\n", microseconds, t);
    
    /* sleep test */
    gettimeofday(&t1, NULL);
    sleep(1);
    gettimeofday(&t2, NULL);
	
    t = ((t2.tv_sec * 1000000) + t2.tv_usec) - ((t1.tv_sec * 1000000) + t1.tv_usec);
    printf("Call to sleep(1) took %lld\n", t);

    return 0;
}
