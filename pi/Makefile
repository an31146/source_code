CC=g++
LD=g++
CFLAGS=-g -O2 -IC:/MinGW/include -Wall -fopenmp
LIBS=  -LC:MinGW/lib -lgomp
EXAMPLES=parallel_pi

all: $(EXAMPLES) 

parallel_pi.o: 
	$(CC) $(CFLAGS) -o parallel_pi.o -c parallel_pi.cpp

parallel_pi: parallel_pi.o
	$(CC) -o parallel_pi parallel_pi.o $(LIBS)

clean:	
	rm $(EXAMPLES) *.o
