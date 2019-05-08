/*
 * mpicc -Wall -O2 -std=c99 -o mpi_broadcast_ring mpi_broadcast_ring.c -lmpi -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

int main( argc, argv )
int argc;
char **argv;
{
    int rank, size;
    char str_value[256] = "";
    float value;
    MPI_Status status;

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    //do {
        if (rank == 0) {
            scanf( "%s", str_value );
	    value = atof(str_value);
            MPI_Send( &value, 1, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD );
        } else {
            MPI_Recv( &value, 1, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, &status );
            if (rank < size - 1)
                MPI_Send( &value, 1, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD );
        }
        printf( "Process %4d got %f\n", rank, value );
    //} while (value >= 0);

    MPI_Finalize( );
    return 0;
}
