/*
<<<<<<< HEAD
  Example Code - Pthread Creation and Termination 
  
  gcc.exe -Wall -O2 std=gnu99 -o pthread_example.exe pthread_example.c -lpthread
*/
=======
 * Example Code - Pthread Creation and Termination 
 * 
 * gcc -Wall -O2 std=gnu99 -o pthread_example pthread_example.c -lpthread
 */
>>>>>>> c9927b723f2cb399085be2360f2ae56e793e3691
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif

<<<<<<< HEAD
#define NUM_THREADS     1000
=======
#define NUM_THREADS     64
>>>>>>> c9927b723f2cb399085be2360f2ae56e793e3691

pthread_mutex_t lock;

void *PrintHello(void *threadid)
{
    pthread_mutex_lock(&lock);

    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    usleep(10000);
   
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init failed\n");
        return -1;
    }

    for(t=0; t<NUM_THREADS; t++) {
        printf("In main: creating thread %ld\n", t);
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
}
