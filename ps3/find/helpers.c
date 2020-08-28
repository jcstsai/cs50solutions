/**************************************************************************** 
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 ***************************************************************************/

#define LIMIT 65536

#include <cs50.h>
#include <string.h>
#include "helpers.h"

#include <stdio.h>


/*
 * bool
 * search(int value, int array[], int n)
 *
 * Returns true if value is in array of n values, else false.
 */

bool 
search(int value, int array[], int n)
{
    int startIndex = 0;
    int endIndex = n;
    int currentIndex;
    int currentElt;
    
    while (startIndex < endIndex) {
        currentIndex = (startIndex+endIndex)/2;
        currentElt = array[currentIndex];
        if (currentElt < value) {
            startIndex = currentIndex + 1;
        } else if (currentElt > value) {
            endIndex = currentIndex;
        } else {
            return true;
        }
    }
    
    return false;
}


/*
 * void
 * sort(int values[], int n)
 *
 * Sorts array of n values.
 */

void 
sort(int values[], int n)
{
    int *newlist = malloc(n * sizeof(int));
    int newlistIndex = 0;
    
    for (int i = 0; i < LIMIT; i++) {
        for (int j = 0; j < n; j++) {
            if (values[j] == i) {
                newlist[newlistIndex] = i;
                newlistIndex++;
            }
        }
        if (newlistIndex == n) break;
    }
    newlist[n] = '\0';
    
    values = (int *) memcpy(values, newlist, n * sizeof(int));
    free(newlist);
    
    return;
}
