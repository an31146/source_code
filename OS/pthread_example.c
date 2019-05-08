/*
  Example Code - Pthread Creation and Termination 
  
  gcc.exe -Wall -O2 -o pthread_example.exe pthread_example.c -lpthread
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif

#define __USE_MINGW_ANSI_STDIO 1

#define NUM_THREADS     100

pthread_mutex_t lock;
int rand_array[NUM_THREADS];

void *PrintHello(void *threadid)
{
    //pthread_mutex_lock(&lock);

    int tid = (int)threadid;
    int random_variable = rand_array[tid];
    printf("Hello World!  It's me, thread #%d!\trand_array = %d\n", tid, random_variable);
    //pthread_mutex_unlock(&lock);

    //pthread_mutex_lock(&lock);
    Sleep((random_variable % 100) * 500 + 100);
    //Sleep(5000);

    printf("Goodbye World!  Thread #%d is exiting!\n", tid);
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    int t;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init failed\n");
        return -1;
    }

    srand(time(NULL)); // use current time as seed for random generator

    for(t=0; t<NUM_THREADS; t++) {
        rand_array[t] = rand();
        printf("In main: creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    for(t=0; t<NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&lock);

    /* Last thing that main() should do */
    pthread_exit(NULL);
    
    return 0;
}
