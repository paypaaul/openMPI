#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "types.h"
#include <time.h>

#define PRIME_NUMBER 7

uint64_t hash_code(char *str, int len) {
    unsigned long long hash = 0;
    for (int i = 0; i < len; i++) {
        hash = (hash * PRIME_NUMBER + str[i]) % 257;
    }
    return hash;
}

void rabin_karp(status_t *status, char *pattern, char *str) {
    uint32_t pat_hash;
    uint32_t str_hash;
    unsigned long long str_len = strlen(str);
    int pat_len = strlen(pattern);
    unsigned long long i, j = 0;
    // puts("rabin karping...");
    // printf("pattern lenght: %d\n", pat_len); 
    // printf("source lenght: %d\n",str_len);
    if(strlen(pattern)>strlen(str)){
        status->current_err = UNCOMPATIBLE_SIZES;
        puts("pattern lenght is greater than source one");
        exit(-1);
    }
    // printf("\n%s\n %s\n", str, pattern);
    pat_hash = hash_code(pattern, pat_len);
    
    for(i = 0; i <= str_len-pat_len;i++) {
        status->current_err = NONE;
        // if(i%100000000==0){
            // printf("entered in loop: %llu\n",i);
        // }
        str_hash = hash_code(str+i, pat_len);
        if(pat_hash == str_hash){
            // pattern detected, checking collision...
            for (j = 0; j < pat_len; j++){
                if(pattern[j] != str[i+j]){
                    status->current_err = HASH_COLLISION;
                    // collision detected and avoided
                    break;
                }
            }
            if(status->current_err != HASH_COLLISION) {

                // match found
                // puts("match found");
                // if(status->freq >= sizeof(status->match_indexes)){
                //     status->match_indexes = realloc(status->match_indexes, 2*sizeof(status->match_indexes));
                // }
                // status->match_indexes[status->freq] = i;
                status->freq = status->freq + 1;
            }
        }
    }

    if(status->freq==0){
        status->current_err = NOT_FOUND;
        // pattern not found in the string
    }
}