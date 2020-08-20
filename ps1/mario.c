#include <stdio.h>
#include <cs50.h>

int main(int argc, char *argv[]) {
    printf("Number: ");
    int height = GetInt();
    while (height > 23 || height < 0) {
        printf("Retry: ");
        height = GetInt();
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < height - i - 1; j++) printf(" ");
        for (int j = 0; j < i + 1; j++) printf("*");
        printf("  ");
        for (int j = 0; j < i + 1; j++) printf("*");
        for (int j = 0; j < height - i - 1; j++) printf (" ");
        printf("\n");
    }
    
    return 0;
}