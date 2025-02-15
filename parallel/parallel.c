#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <mpi.h>	
// #include "types.h"
#include "utilities.h"
#include "rabin_karp.h"

#define PRIME_NUMBER 7
#define _LARGEFILE64_SOURCE

char *read_file(char file_name[]) {  

    FILE *file_in;
    char buff[1000];
    char *result;
    int count = 0;
    file_in = fopen(file_name,"r");
    result = malloc((UINT32_MAX)*sizeof(char));
    int size;

    while(fgets(buff, sizeof(buff), file_in)) {
        if(count ==0){
            size = strlen(buff);
        }
        memcpy(result+(count*size), buff, size); // memcpy() (- count to exclude the \n )
        count++;
        size = strlen(buff);
    }
    
    puts("read done");
    result = realloc(result, strlen(result));
    fclose(file_in);

    return result;
}

int main (int argc, char *argv[]) {

	int myrank, n_cores, err, size;
	MPI_Status MPIstatus;
	MPI_Offset txt_len;
	MPI_File file_in;
	char *pat_name = "data/pattern-10.txt";
	char *text_name = "data/data2.txt";
	int reminder;
	char *pattern;
	char *txt_block;
	int pat_len;
	int myfreq;
	int *freq_vec;
	long int block_size;
	long int msg[4];
	status_t status;
	clock_t start_time, end_time;

	
	
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
  	MPI_Comm_size(MPI_COMM_WORLD,&n_cores);
	if(myrank == 0){

		
	}

	err = MPI_File_open(MPI_COMM_SELF, text_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &file_in);
    if (err != MPI_SUCCESS) {
        fprintf(stderr, "Error opening file %s\n", text_name);
        MPI_Abort(MPI_COMM_SELF, err);
    } else {
        // printf("File %s opened successfully\n", text_name);
    }

  	MPI_File_get_size(file_in, &txt_len);
  	txt_len = txt_len-1;     
	// printf("myrank: %d\n", myrank);
	// printf("dim :%d\n", size);
	status.current_err = NONE;
	status.freq = 0;
	status.match_indexes = malloc(10*sizeof(uint32_t));

	if(myrank == 0) {
		size = n_cores;
		pattern = read_file(pat_name);
		pat_len = strlen(pattern);
		printf("file dim: %lld - pattern dim: %d\n", txt_len, pat_len);
		block_size = txt_len / size;
	
		while(block_size < pat_len) {
			size--;
			block_size = txt_len / size;
		}
			
		reminder = txt_len % size;

		printf("pattern:%s",pattern);
		msg[0] = block_size; //block_dim + patlen - 1 cioe quanto analizza ognuno
		msg[1] = pat_len;	//lunghezza pattern
		msg[2] = reminder;	//resto
		msg[3] = size;		//core attivi

	}


  	MPI_Bcast(msg, 4, MPI_LONG, 0, MPI_COMM_WORLD);
	
	block_size = msg[0];
	pat_len = msg[1];
	reminder = msg[2];
	size = msg[3];
	if(myrank!=0 ){
		pattern = malloc(pat_len*sizeof(char));
	}
	MPI_Bcast(pattern, pat_len, MPI_CHAR, 0, MPI_COMM_WORLD);

	if(myrank>size){
		status.current_err = CORE_IDLE;
	}
	
	if(status.current_err != CORE_IDLE){


		txt_block = pinch_block(file_in, block_size, size, reminder, myrank, pat_len, MPIstatus);

		MPI_Barrier(MPI_COMM_WORLD);

		if(myrank == 0){
			start_time = clock();
		}
		rabin_karp(&status, pattern, txt_block);
		if(myrank == 0){
			end_time = clock();
		}
		free(pattern);
		free(txt_block);

		if(myrank == 0) {
			freq_vec = (int*)malloc(sizeof(int)*size);
		}

		MPI_Barrier(MPI_COMM_WORLD);

		// printf("\n\nRESULTS\n\n process %d - frequency %d", myrank, status.freq);

		myfreq = status.freq;
		MPI_Gather(&myfreq, 1, MPI_INT, freq_vec, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		if(myrank == 0) {
			int t_frequency = 0;
			for(int i = 1; i<size;i++){
      			t_frequency = t_frequency + freq_vec[i];
    		}
			status.freq += t_frequency;
		}

	}
	if(myrank == 0){

		printf("\n\nRESULTS\n\n TOTAL FREQUENCY = %d ", status.freq);
		// clock_t end_time = clock();
		double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		printf("Execution time: %.5f seconds\n", time_spent);
	}

	MPI_File_close(&file_in);
	MPI_Finalize(); 
	return 0;
}
