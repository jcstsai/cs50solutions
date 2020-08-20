#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <unistd.h>

int generate_strings(char *input, int length, char letters[], char *prefix, char salt[]);

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
    char lower_letters[26] = "abcdefghijklmnopqrstuvwxyz";
    char upper_letters[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    sprintf(guess, "%s", "");
    for (int i = 0; i < 6; i++) {
        printf("%d\n", i);
        if (generate_strings(input, i+1, lower_letters, "", salt) == 0) {
            return 0;
        }
        if (generate_strings(input, i+1, upper_letters, "", salt) == 0) {
            return 0;
        }
    }
        
    // check all combinations upper- and lowercase characters, numbers, and symbols
    char symbols[94] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890`~!@#$%^&*()-_+=[{]}\\|;:'\",<.>/?";
    sprintf(guess, "%s", "");
    for (int i = 0; i < 9; i++) {
        printf("%d\n", i);
        if (generate_strings(input, i+1, symbols, "", salt) == 0) {
            return 0;
        }
    }
        
    printf("Failed to crack the password.\n");
    return 2;
}

/**
 * Recursively generate all strings from the given set of letters.
 */
int generate_strings(char *input, int length, char letters[], char *prefix, char salt[]) {
    if (length == 0) {
        if (strcmp(input, crypt(prefix, salt)) == 0) {
            printf("%s\n", prefix);
            return 0;
        } else {
            printf("%s\n", prefix);
            return 1;
        }
    } else {
        for (int i = 0; i < 74; i++) {
            char newLetter = letters[i];
            char *newString = malloc(sizeof(char) * (strlen(prefix) + 1));
            strncpy(newString, prefix, strlen(prefix));
            newString[strlen(prefix)] = newLetter;
            newString[strlen(prefix)+1] = '\0';
            if (generate_strings(input, length - 1, letters, newString, salt) == 0) {
                return 0;
            }
            free(newString);
        }
        return 1;
    }
}
