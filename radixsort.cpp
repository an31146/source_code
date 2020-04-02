/*
 * g++ -Wall -O2 -std=c++11 -o radixsort.exe radixsort.cpp
 */
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIMIT 4000

using namespace std;

// http://cubic.org/docs/radix.htm

void radix (int byte, long N, long *source, long *dest)
{
  long count[1024];
  long index[1024];
  int  i;
  memset (count, 0, sizeof (count));
  for ( i=0; i<N; i++ ) count[((source[i])>>(byte*8))&0xff]++;

  index[0]=0;
  for ( i=1; i<1024; i++ ) 
	  index[i]=index[i-1]+count[i-1];
  for ( i=0; i<N; i++ ) 
	  dest[index[((source[i])>>(byte*8))&0xff]++] = source[i];
}

void radixsort (long *source, long *temp, long N)
{
  radix (0, N, source, temp);
  radix (1, N, temp, source);
  radix (2, N, source, temp);
  radix (3, N, temp, source);
}

void make_random (long *data, long N)
{
  srand(time(NULL));
  for ( int i=0; i<N; i++ ) 
  {
    data[i]=rand() % LIMIT;
    cout << setw(5) << data[i] << "   ";
    //printf("%5d", data[i]);
  }
  cout << "\n\n\n";
}

long data[LIMIT];
long temp[LIMIT];

int main (void)
{
  make_random(data, LIMIT);
  radixsort (data, temp, LIMIT);
  for ( int i=0; i<LIMIT; i++ ) 
    cout << setw(5) << data[i];
    //printf("%5d", data[i]);
  cout << endl;
  return 0;
}
