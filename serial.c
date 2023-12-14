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

    pat_hash = hash_code(pattern, pat_len);

    for(i = 0; i < str_len-pat_len;i++) {
        str_hash = hash_code(str+(i*pat_len), pat_len);

        if(pat_hash == str_hash){
            puts("pattern detected, checking collision...");

            for (j = 0; j < pat_len; j++){

                if(pattern[j] =! str[(i*pat_len)]){

                    status->current_err = HASH_COLLISION;
                    puts("collision detected and avoided");
                    break;
                }
            }
            if(status->current_err=! HASH_COLLISION) {

                printf("match found at %d",i*pat_len);
                status->match_indexes[status->freq] = i*pat_len;
                status->freq = status->freq + 1;
            }
        }
    }
    if(status->freq==0){
        status->current_err = NOT_FOUND;
        puts("pattern not found in the string");
    }
}

char *read_file(char file_name[]) {  

  FILE *file_in;
  char *buf;
  file_in = fopen64(file_name,"rb");
  buf = (char*) malloc(UINT32_MAX*sizeof(char));

  while(fread(buf,sizeof(char),UINT32_MAX,file_in)>0);

  buf = realloc(buf, strlen(buf));

  fclose(file_in);

  return buf;

}

int main(int argc, char **argv) {



    return 0;
}

