#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "types.h"
#include <time.h>
#define PRIME_NUMBER 101

uint64_t hash_code(char *str, int len) {
    unsigned long long hash = 0;
    for (int i = 0; i < len; i++) {
        hash = (hash * PRIME_NUMBER + str[i]) % INT32_MAX;
    }
    return hash;
}

void rabin_karp(status_t *status, char *pattern, char *str, int n_cores, int p_rank) {
    uint64_t pat_hash;
    uint64_t str_hash;
    int str_len = strlen(str)/n_cores;
    int pat_len = strlen(pattern);
    int i, j = 0;
    char *start = str + p_rank*(str_len);


    pat_hash = hash_code(pattern, pat_len);
    
    for(i = 0; i <= str_len-pat_len;i++) {
        str_hash = hash_code(start+i, pat_len);
        if(pat_hash == str_hash){
            // pattern detected, checking collision...
            for (j = 0; j < pat_len; j++){
                if(pattern[j] != start[i+j]){
                    status->current_err = HASH_COLLISION;
                    // collision detected and avoided
                    break;
                }
            }
            if(status->current_err != HASH_COLLISION) {

                // match found
                status->match_indexes[status->freq] = i;
                status->freq = status->freq + 1;
            }
        }
    }
    if(status->freq==0){
        status->current_err = NOT_FOUND;
        // pattern not found in the string
    }
}