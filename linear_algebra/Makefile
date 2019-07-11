CC=gcc
LD=gcc
CFLAGS=-Wall -O2 -std=c99
LIBS=-lm
EXAMPLES=doolittle_1 doolittle_2 gauss_1 gauss_2

all: $(EXAMPLES)

doolittle_1:
	$(CC) $(CFLAGS) -o doolittle_1 doolittle_1.c doolittle.c unit_lower_triangular.c upper_triangular.c

doolittle_2:
	$(CC) $(CFLAGS) -o doolittle_2 doolittle_2.c doolittle_pivot.c unit_lower_triangular.c upper_triangular.c

gauss_1:
	$(CC) $(CFLAGS) -o gauss_1 gauss_1.c gauss_elimination.c $(LIBS)

gauss_2:
	$(CC) $(CFLAGS) -o gauss_2 gauss_2.c gauss_aux_elimination.c

clean:
	rm $(EXAMPLES)
