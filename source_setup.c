#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

void correct_file(char file_name_out[], char file_name_in[]) {  

    FILE *file_out;
    FILE *file_in;
    char buff[200];
    int valid = 0;
    file_out = fopen(file_name_out,"w+");
    file_in = fopen(file_name_in,"r");

    while(fgets(buff, sizeof(buff), file_in)) {
        // printf("\nBUFF\n%s\n", buff);
        // printf("\ncount: %d, size: %d", count, size);
        for(int k=0;k<strlen(buff)-1;k++) {
            if(buff[k]!='N')
                valid=1;
            if(buff[0]=='>') {
                valid=0;
            }
        }
        if(valid)
            fprintf(file_out, "%s", buff);

        valid = 0;

    }
    
    puts("done");
    fclose(file_in);
    fclose(file_out);
}

int main(int argc, char **argv) {

    correct_file("data.txt","GRCh37_latest_genomic.fna");

    return 0;
}