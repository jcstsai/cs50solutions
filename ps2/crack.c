#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <unistd.h>

int generate_strings(char *input, char *salt, int currentPos, int length, char *letters, int lettersLength, char *prefix);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please enter exactly one argument.\n");
        return 1;
    }
    
    char *input = argv[1];
    char salt[2] = { (char) 53, (char) 48 }; // found empirically
    char guess[9];
    
    // Attempt to check all dictionary words
    FILE *fp = fopen("words.txt", "r");
    char buf[255];

    while (!feof(fp)) {
        fscanf(fp, "%s", buf);
        if (strcmp(input, crypt(buf, salt)) == 0) {
            printf("%s\n", buf);
            return 0;
        }
    }
    
    fclose(fp);
    
    // Attempt to check all <= 5 digit numbers
    for (long i = 0; i < 100000; i++) {
        sprintf(guess, "%ld", i);
        if (strcmp(input, crypt(guess, salt)) == 0) {
            printf("%s\n", guess);
            return 0;
        }
    }
    
    // Attempt to check all combinations of letters
    char letters[52] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 6; i++) {
        char *prefix = malloc(sizeof(char) * (i+2));
        if (generate_strings(input, salt, 0, i+1, letters, 52, prefix) == 0) {
            return 0;
        }
        free(prefix);
    }
        
    // check all combinations upper- and lowercase characters, numbers, and symbols
    char symbols[94] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890`~!@#$%^&*()-_+=[{]}\\|;:'\",<.>/?";
    sprintf(guess, "%s", "");
    for (int i = 0; i < 9; i++) {
        char *prefix = malloc(sizeof(char) * (i+2));

        if (generate_strings(input, salt, 0, i+1, symbols, 94, prefix) == 0) {
            free(prefix);
            return 0;
        }
        free(prefix);
    }
        
    printf("Failed to crack the password.\n");
    return 2;
}

/**
 * Recursively generate and check all strings of a given length from the given set of letters.
 */
int generate_strings(
                     char *input,
                     char *salt,
                     int currentPos,
                     int length,
                     char *letters,
                     int lettersLength,
                     char *prefix) {
    if (currentPos == length) {
        prefix[currentPos] = '\0';
        if (strcmp(input, crypt(prefix, salt)) == 0) {
            printf("%s\n", prefix);
            return 0;
        } else {
            return 1;
        }
    } else {
        for (int i = 0; i < lettersLength; i++) {
            prefix[currentPos] = letters[i];
            if (generate_strings(input, salt, currentPos + 1, length, letters, lettersLength, prefix) == 0) {
                return 0;
            }
        }
        return 1;
    }
}
