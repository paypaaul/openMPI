#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <mpi.h>
#include "types.h"
#define PRIME_NUMBER 101
#define _LARGEFILE64_SOURCE

//git add
//git commit -m *insert message*
//git push

void main (int argc, char *argv[]) {
	int myrank, size;
	/* 1. Initialize MPI */
	MPI_Init(&argc, &argv);
	/* 2. Get my rank */ MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	/* 3. Get the total number of processes */ MPI_Comm_size(MPI_COMM_WORLD, &size);
	/* 4. Print myrank and size */ printf("Process %d of %d \n", myrank, size); 
	/* 5. Terminate MPI */ MPI_Finalize ();
}
