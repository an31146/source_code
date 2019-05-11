/*
  Example Code - Pthread Creation and Termination 
  
  gcc.exe -Wall -O2 -o pthread_example.exe pthread_example.c -lpthread
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif

#define __USE_MINGW_ANSI_STDIO 1

#define NUM_THREADS     100

pthread_mutex_t lock;

void *PrintHello(void *threadid)
{
    pthread_mutex_lock(&lock);

    long long tid;
    tid = (long long)threadid;
    Sleep(200);
    printf("Hello World! It's me, thread #%lld!\n", tid);
   
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long long t;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init failed\n");
        return -1;
    }

    for(t=0; t<NUM_THREADS; t++) {
        printf("In main: creating thread %lld\n", t);
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
