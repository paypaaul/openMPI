#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "types.h"

#define PRIME_NUMBER 101
#define _LARGEFILE64_SOURCE

uint64_t hash_code(char *str, int len) {
    unsigned long long hash = 0;
    for (int i = 0; i < len; i++) {
        hash = (hash * PRIME_NUMBER + str[i]) % UINT_MAX;
    }
    return hash;
}

void rabin_karp(status_t *status, char *pattern, char *str) {
    uint64_t pat_hash;
    uint64_t str_hash;
    int str_len = strlen(str);
    int pat_len = strlen(pattern);
    int i, j = 0;

    if(strlen(pattern)>strlen(str)){
        status->current_err = UNCOMPATIBLE_SIZES;
        puts("pattern lenght is greater than source one");
        exit(-1);
    }

    pat_hash = hash_code(pattern, pat_len);
    // printf("\n%s - %llu\n\n", pattern, pat_hash);
    
    
    for(i = 0; i <= str_len-pat_len;i++) {
        str_hash = hash_code(str+i, pat_len);
        // printf("\nround: %d\n\thash: %llu\n\t", i, str_hash);

        // for(int k=0;k<pat_len;k++) {
        //     printf("%c", str[i+k]);
        // }

        if(pat_hash == str_hash){
            puts("pattern detected, checking collision...");

            for (j = 0; j < pat_len; j++){

                if(pattern[j] != str[i+j]){

                    status->current_err = HASH_COLLISION;
                    puts("collision detected and avoided");
                    break;
                }
            }
            if(status->current_err != HASH_COLLISION) {

                printf("\nmatch found at index: %d\n",i);
                status->match_indexes[status->freq] = i;
                status->freq = status->freq + 1;
            }
        }
    }
    if(status->freq==0){
        status->current_err = NOT_FOUND;
        puts("\npattern not found in the string");
    }
}

char *read_file(char file_name[]) {  

    FILE *file_in;
    char buff[200];
    char *result;
    int count = 0;
    file_in = fopen(file_name,"r");
    result = malloc(10000*sizeof(char));
    int size;

    while(fgets(buff, sizeof(buff), file_in)) {

        if(count ==0)
            size = strlen(buff);

        memcpy(result+(count*size)-count , buff, size);
        count++;
        size = strlen(buff);

    }
    
    result = realloc(result, strlen(result));

    fclose(file_in);

    return result;
}

int main(int argc, char **argv) {

    status_t state;
    char *source; 
    char *pattern;

    state.current_err= NONE;
    state.freq=0;
    state.match_indexes = malloc(10*sizeof(uint32_t));

    source = read_file("sample02.txt");
    pattern = read_file("pattern02.txt");
    // printf("\n\n%s\n", source);
    // printf("\n\n%s\n", pattern);

    rabin_karp(&state, pattern, source);

    free(source);
    free(pattern);

    return 0;
}

