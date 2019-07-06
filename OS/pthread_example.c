/*
 * Example Code - Pthread Creation and Termination 
 * 
 * gcc.exe -Wall -O2 -o pthread_example.exe pthread_example.c -lpthread
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

<<<<<<< HEAD
<<<<<<< HEAD
#define NUM_THREADS     50
=======
#define NUM_THREADS     64
>>>>>>> refs/rewritten/onto
=======
#define NUM_THREADS     64
>>>>>>> 159d585d04a2939e4f0ab1e0dbed8921af17ed9f

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int rand_array[NUM_THREADS];

void *PrintHello(void *threadid)
{
    //pthread_mutex_lock(&lock);

    int tid = (int)threadid;
    int random_variable = rand_array[tid];
    
    pthread_mutex_lock(&lock);
    printf("Hello World!  It's me, thread #%d!\nrand_array[%d] = %d\n", tid, tid, random_variable);
    pthread_mutex_unlock(&lock);

    int delay_ms = (random_variable % 100) * 500 + 500;
    Sleep(delay_ms);

    pthread_mutex_lock(&lock);
    printf("Goodbye Cruel World!  Thread #%02d is exiting after %d ms!\n", tid, delay_ms);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init failed\n");
        return -1;
    }

    srand(time(NULL)); // use current time as seed for random generator

    for(int t = 0; t < NUM_THREADS; t++) {
        rand_array[t] = rand();
        
        pthread_mutex_lock(&lock);
        printf("In main: creating thread %d\n\n", t);
        pthread_mutex_unlock(&lock);
        
        int rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    for(int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&lock);

    /* Last thing that main() should do */
    pthread_exit(NULL);
    
    return 0;
}
