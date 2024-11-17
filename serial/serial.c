#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "types.h"
#include <time.h>
#define PRIME_NUMBER 7
#define _LARGEFILE64_SOURCE

uint64_t hash_code(char *str, int len) {
    unsigned long long hash = 0;
    for (int i = 0; i < len; i++) {
        hash = (hash * PRIME_NUMBER + str[i]) % 1000003;
    }
    return hash;
}

void rabin_karp(status_t *status, char *pattern, char *str) {
    uint64_t pat_hash;
    uint64_t str_hash;
    long int str_len = strlen(str);
    long int pat_len = strlen(pattern);
    printf("\nSTRLEN: %ld\n", str_len);
    long int i, j = 0;

    if(strlen(pattern)>strlen(str)){
        status->current_err = UNCOMPATIBLE_SIZES;
        puts("pattern lenght is greater than source one");
        exit(-1);
    }

    pat_hash = hash_code(pattern, pat_len);
    // printf("\n%s - %llu\n\n", pattern, pat_hash);
    
    
    for(i = 0; i <= str_len-pat_len;i++) {

        status->current_err = NONE;
        str_hash = hash_code(str+i, pat_len);
        // printf("\nround: %d\n\thash: %llu\n\t", i, str_hash);

        // for(int k=0;k<pat_len;k++) {
        //     printf("%c", str[i+k]);
        // }

        if(pat_hash == str_hash){
            // puts("pattern detected, checking collision...");

            for(j = 0; j < pat_len; j++){

                if(pattern[j] != str[i+j]){

                    status->current_err = HASH_COLLISION;
                    // puts("collision detected and avoided");
                    break;
                }
            }
            if(status->current_err != HASH_COLLISION) {

                // printf("\nmatch found at index: %d\n",i);
                // if(status->freq >= sizeof(status->match_indexes)){
                //     status->match_indexes = realloc(status->match_indexes, 2*sizeof(status->match_indexes));
                // }
                // status->match_indexes[status->freq] = i;
                status->freq = status->freq + 1;
            }
        }
    }
    printf("final i: %ld ",i);
    if(status->freq==0){
        status->current_err = NOT_FOUND;
        puts("\npattern not found in the string");
    }
}

char *read_file(char file_name[]) {  

    FILE *file_in;
    char buff[1000];
    char *result;
    long int count = 0;
    // int dim = 2;
    file_in = fopen(file_name,"r");
    if (file_in == NULL) {
        perror("error opening the file");
        exit(-2);
    }
    result = malloc((UINT32_MAX)*sizeof(char));
    long int size;
    // puts("read started");
    
    while(fgets(buff, sizeof(buff), file_in)) {
        // printf("\nBUFF\n%s\n", buff);
        // if(count%100000==0){
        //     printf("\ncount: %d, size: %d", count, size);
        // }
        if(count ==0){
            size = strlen(buff);
            // result = malloc(dim*size*sizeof(char));
        }
        // printf("\n\n offset result: %d", (count*size)-count);
        // printf("\n\n size of result: %lu", sizeof(result));
        memcpy(result+(count*size)-count , buff, size); // memcpy()
        // printf("%d\n",count);
        
        count++;
        size = strlen(buff);
        // printf("\nsize: %d, dim: %d, count: %d\n", size, dim , count);
        // if(size*dim<size*count)
        //     dim*=2;
        //     result = realloc(result, dim*size*sizeof(char));
        // if(count>26512142)
        //     break;


    }
    
    puts("read done");
    printf("count rows: %ld", count);
    result = realloc(result, strlen(result));
    fclose(file_in);

    return result;
}

int main(int argc, char **argv) {

    clock_t start_time, end_time;
    status_t state;
    char *source; 
    char *pattern;

    state.current_err= NONE;
    state.freq=0;
    // state.match_indexes = malloc(10*sizeof(uint32_t));

    pattern = read_file("data/test-pattern.txt");
    source = read_file("data/data2.txt");
    printf("\n\n%ld\n", strlen(source));
    printf("\n\n%ld\n", strlen(pattern));
    start_time = clock();
    rabin_karp(&state, pattern, source);
    // state.match_indexes = realloc(state.match_indexes, state.freq);
    end_time = clock();

    printf("\n----RESULTS----\n\tNumber of matches: %d\n\t\t Indexes:\n\t\t\t", state.freq);
    // for(int k=0; k<state.freq;k++){

    
        // printf("%u, ",state.match_indexes[k]);

    // }

    free(source);
    free(pattern);

    
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.5f seconds\n", time_spent);
    return 0;
}
