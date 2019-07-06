#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <pthread.h> // pthread.h in POSIX
 
// the function f() does some time-consuming work
void *func(void* thr_data) // return void* in POSIX
{
    volatile double d = 0;
    for (int n=0; n<10000; ++n)
       for (int m=0; m<10000; ++m)
           d += d*n*m;
    return 0;
}
 
int main(void)
{
    struct timespec ts1, tw1; // both C11 and POSIX
    timespec_get(&ts1, TIME_UTC); // POSIX
    timespec_get(&tw1, 0); // POSIX; use timespec_get in C11
    clock_t t1 = clock();
 
    pthread_t thr1, thr2;  // C11; use pthread_t in POSIX
    pthread_create(&thr1, NULL, func, NULL); // C11; use pthread_create in POSIX
    pthread_create(&thr2, NULL, func, NULL);
    pthread_join(thr1, NULL); // C11; use pthread_join in POSIX
    pthread_join(thr2, NULL);
 
    struct timespec ts2, tw2;
    timespec_get(&ts2, TIME_UTC);
    timespec_get(&tw2, 0);
    clock_t t2 = clock();
 
    double dur = 1000.0*(t2-t1)/CLOCKS_PER_SEC;
    double posix_dur = 1000.0*ts2.tv_sec + 1e-6*ts2.tv_nsec
                       - (1000.0*ts1.tv_sec + 1e-6*ts1.tv_nsec);
    double posix_wall = 1000.0*tw2.tv_sec + 1e-6*tw2.tv_nsec
                       - (1000.0*tw1.tv_sec + 1e-6*tw1.tv_nsec);
 
    printf("CPU time used (per clock(): %.2f ms\n", dur);
    printf("CPU time used (per clock_gettime()): %.2f ms\n", posix_dur);
    printf("Wall time passed: %.2f ms\n", posix_wall);
}
