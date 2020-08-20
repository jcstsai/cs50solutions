#include <stdio.h>
#include <cs50.h>
#include <math.h>

/**
 * Algorithm:
 * Compute the greedy solution, then take off one quarter
 * and see if we can come up with a better one.
 * It is easily shown that the greedy solution works if
 * there are no quarters, or if there are no dimes, so
 * the only case we need worry about is the case of having
 * quarters and dimes together. 
 * Clearly two quarters being replaced by five dimes is
 * suboptimal, so we need only worry about replacing one
 * quarter.
 */
int main(int argc, char *argv[]) {
    // get inputs
    printf("O hai!\n");
    printf("Number of quarters: ");
    int quarters = GetInt();
    while (quarters < 0) {
        printf("Retry: ");
        quarters = GetInt();
    }
    printf("Number of dimes: ");
    int dimes = GetInt();
    while (dimes < 0) {
        printf("Retry: ");
        dimes = GetInt();
    }
    printf("Number of nickels: ");
    int nickels = GetInt();
    while (nickels < 0) {
        printf("Retry: ");
        nickels = GetInt();
    }
    printf("Number of pennies: ");
    int pennies = GetInt();
    while (pennies < 0) {
        printf("Retry: ");
        pennies = GetInt();
    }
    printf("Change owed: ");
    float centsInput = GetFloat();
    while (centsInput < 0.0) {
        printf("Retry: ");
        centsInput = GetFloat();
    }
    int cents = (int) round(centsInput * 100.0);
    int altCents = cents;
    int altQuarters = quarters;
    int altDimes = dimes;
    int altNickels = nickels;
    int altPennies = altPennies;
    
    // method 1: greedy
    int coins = 0;
    while (cents >= 25 && quarters > 0) {
        cents -= 25;
        coins += 1;
        quarters -= 1;
    }
    while (cents >= 10 && dimes > 0) {
        cents -= 10;
        coins += 1;
        dimes -= 1;
    }
    while (cents >= 5 && nickels > 0) {
        cents -= 5;
        coins += 1;
        nickels -= 1;
    }
    while (cents >= 1 && pennies > 0) {
        cents -= 1;
        coins += 1;
        pennies -= 1;
    }
    
    // method 2: ignore the last quarter
    int altCoins = 0;
    while (altCents >= 50 && altQuarters > 0) {
        altCents -= 25;
        altCoins += 1;
        altQuarters -= 1;
    }
    while (altCents >= 10 && altDimes > 0) {
        altCents -= 10;
        altCoins += 1;
        altDimes -= 1;
    }
    while (altCents >= 5 && altNickels > 0) {
        altCents -= 5;
        altCoins += 1;
        altNickels -= 1;
    }
    while (altCents >= 1 && altPennies > 0) {
        altCents -= 1;
        altCoins += 1;
        altPennies -= 1;
    }
    
    // override greedy answer with alternate answer if it's better or greedy failed
    if (cents > 0) coins = 0;
    if (altCoins < coins || (cents > 0 && altCents == 0)) coins = altCoins;
    printf("%d\n", coins);
}