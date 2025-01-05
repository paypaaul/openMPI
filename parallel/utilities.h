#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <mpi.h>

char *pinch_block(MPI_File file,long int block_size,int n_block, int reminder, int rank, int pat_len, MPI_Status status);

char *pinch_block(MPI_File file, long int block_size,int n_block, int reminder, int rank, int pat_len, MPI_Status status){

    // printf("process %d reminder: %d\n", rank,rem_flag);
    char *block = malloc((block_size+pat_len)*sizeof(char));
    *(block+block_size+pat_len-1) = '\0';

    long int offset = (block_size)*rank;
    long int read_dim = (rank == n_block - 1) ? (block_size + reminder) : (block_size + pat_len - 1);


    printf("\n\n******** myrank: %d, block_size:%ld offset: %ld, read_dim: %ld\n\n",rank,block_size, offset, read_dim);
    int err1 = MPI_File_set_view(file, offset, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);

    int err2 = MPI_File_read(file, block, read_dim, MPI_CHAR, &status);
    // printf("\n**********myrank%d BLOCK: %s\n",rank, block);
    
    // if(err1||err2 != 0){
    //     printf("\n\n\n*******ERROR (%d, %d)*********\n", err1,err2 );
       
    // }

    return block;

}