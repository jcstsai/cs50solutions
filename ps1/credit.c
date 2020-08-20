#include <stdio.h>
#include <cs50.h>
#include <math.h>

int checkLuhn(long long number, int digits);

int main(int argc, char *argv[]) {
    printf("Number: ");
    long number = GetLongLong();
    
    // check > 0
    if (number <= 0) {
        printf("INVALID\n");
        return 0;
    }
    
    // Check number of digits
    int digits = (int) (log(number) / log(10) + 1);
    if (digits != 13 && digits != 15 && digits != 16) {
        printf("INVALID\n");
        return 0;
    }
    
    // Check validity using Luhn's algorithm
    int isValidNumber = checkLuhn(number, digits);
    if (!isValidNumber) {
        printf("INVALID\n");
        return 0;
    }

    // Check beginning digits
    int firstTwoDigits = number / ((long long) pow(10, digits - 2));
    switch (digits) {
        case 13:
            if (firstTwoDigits > 39 && firstTwoDigits < 50)
                printf("VISA\n");
            else
                printf("INVALID\n");
            break;
        case 15:
            if (firstTwoDigits == 34 || firstTwoDigits == 37)
                printf("AMEX\n");
            else
                printf("INVALID\n");
            break;
        case 16:
            if (firstTwoDigits > 50 && firstTwoDigits < 56)
                printf("MASTERCARD\n");
            else if (firstTwoDigits > 39 && firstTwoDigits < 50)
                printf("VISA\n");
            else
                printf("INVALID\n");
            break;
        default:
            printf("INVALID\n");
    }
    
    return 0;
}

int checkLuhn(long long number, int digits) {
    int digitSum = 0;
    
    // i. handle odd digits
    for (int i = 1; i < digits; i += 2) {
        // For the ith digit, truncate number to that number of digits.
        long long truncatedNumber = (number / ((long long) pow(10, i)));
        // Also, truncate the number to i + 1 digits.
        long long truncatedBigNumber = (number / ((long long) pow(10, i+1)));
        
        // Use those truncated numbers to find the ith digit
        int digit;
        if (truncatedBigNumber == 0)
            digit = truncatedNumber;
        else
            digit = truncatedNumber % (truncatedBigNumber * 10);
        
        // Add to the checksum
        digitSum += (digit * 2 / 10 + digit * 2 % 10);
    }
    
    // ii. handle even digits
    for (int i = 0; i < digits; i += 2) {
        // For the ith digit, truncate number to that number of digits.
        long long truncatedNumber = (number / ((long long) pow(10, i)));
        long long truncatedBigNumber = (number / ((long long) pow(10, i+1)));
        
        // Use those truncated numbers to find the ith digit
        int digit;
        if (truncatedBigNumber == 0)
            digit = truncatedNumber;
        else
            digit = truncatedNumber % (truncatedBigNumber * 10);
        
        // Add to the checksum
        digitSum += digit;
    }
    
    
    // iii. check that last digit of digitSum is 0
    if (digitSum % 10 == 0) return 1; else return 0;
}
