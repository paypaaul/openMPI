#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define PRIME_NUMBER 101 // A prime number used for the hash function

unsigned long long hash(char *str, int len) {
    unsigned long long h = 0;
    for (int i = 0; i < len; i++) {
        h = (h * PRIME_NUMBER + str[i]) % UINT_MAX;
    }
    return h;
}

void searchRabinKarp(char *pattern, char *text) {
    int patternLength = strlen(pattern);
    int textLength = strlen(text);

    unsigned long long patternHash = hash(pattern, patternLength);
    unsigned long long textHash = hash(text, patternLength);

    for (int i = 0; i <= textLength - patternLength; i++) {
        if (patternHash == textHash) {
            // If the hash values match, check character by character
            int j;
            for (j = 0; j < patternLength; j++) {
                if (text[i + j] != pattern[j]) {
                    break;
                }
            }
            if (j == patternLength) {
                printf("Pattern found at index %d\n", i);
            }
        }

        // Update the hash value for the next window in the text
        if (i < textLength - patternLength) {
            textHash = (PRIME_NUMBER * (textHash - text[i] * (unsigned long long)pow(PRIME_NUMBER, patternLength - 1)) + text[i + patternLength]) % UINT_MAX;

            // Ensure the hash value is non-negative
            if (textHash < 0) {
                textHash += UINT_MAX;
            }
        }
    }
}

int main() {
    char pattern[100]; // Assuming a maximum pattern length of 100 characters
    printf("Enter the pattern to search: ");
    fgets(pattern, sizeof(pattern), stdin);

    // Removing the newline character if present
    int patternLength = strlen(pattern);
    if (pattern[patternLength - 1] == '\n') {
        pattern[patternLength - 1] = '\0';
    }

    char filename[100]; // Assuming a maximum filename length of 100 characters
    printf("Enter the filename containing the text: ");
    fgets(filename, sizeof(filename), stdin);

    // Removing the newline character if present
    int filenameLength = strlen(filename);
    if (filename[filenameLength - 1] == '\n') {
        filename[filenameLength - 1] = '\0';
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char text[1000]; // Assuming a maximum text length of 1000 characters
    fread(text, sizeof(char), sizeof(text), file);

    // Removing the newline character if present
    int textLength = strlen(text);
    if (text[textLength - 1] == '\n') {
        text[textLength - 1] = '\0';
    }

    fclose(file);

    searchRabinKarp(pattern, text);

    return 0;
}
