/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Define a node for linked lists of words
typedef struct node {
    char *word;
    struct node *next;
} node;

// Build the hashtable of linked lists
node *words[100000];


/*
 * bool
 * check(const char *word)
 *
 * Returns true if word is in dictionary else false.
 */

bool
check(const char *word)
{
    // make a lowercase copy of the word
    char *word_copy = malloc(strlen(word) * sizeof(char));
    word_copy = strcpy(word_copy, word);
    for (int i = 0; i < strlen(word_copy); i++) {
        word_copy[i] = tolower(word_copy[i]);
    }
    
    // get the linked list matching the hash value
    long hashValue = hash(word_copy) % 100000;
    node *slot = words[hashValue];
    
    // search for a match in this linked list
    while (slot->word != NULL) {
        if (strcmp(slot->word, word_copy) == 0) {
            free(word_copy);
            return true;
        }
        slot = slot->next;
    }
    
    // return false if no match was found
    free(word_copy);
    return false;
}


/*
 * bool
 * load(const char *dict)
 *
 * Loads dict into memory.  Returns true if successful else false.
 */

bool
load(const char *dict)
{
    // initialize hash table
    for (int i = 0; i < 100000; i++) {
        words[i] = malloc(sizeof(node));
        words[i]->word = NULL;
        words[i]->next = NULL;
    }
    
    FILE *fp = fopen(dict, "r");
    char *buf;
    long hashValue;
    node *newNode;
    
    // for each word, create a node and prepend it to the linked list
    // in the slot matching its hash value
    while (!feof(fp)) {
        buf = malloc(LENGTH * sizeof(char));
        fscanf(fp, "%s", buf);
        
        hashValue = hash(buf) % 100000;
        
        newNode = malloc(sizeof(node));
        newNode->word = buf;
        newNode->next = words[hashValue];
        
        words[hashValue] = newNode;
        
        words[hashValue]->word = buf;
        
    }
    
    fclose(fp);
    
    return true;
}


/*
 * unsigned int
 * size()
 *
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */

unsigned int
size()
{
    int size = 0;
    node *slot;
    
    // count the words in each slot
    for (int i = 0; i < 100000; i++) {
        slot = words[i];
        while (slot->word != NULL) {
            slot = slot->next;
            size++;
        }
    }
    
    return size;
}


/*
 * bool
 * unload()
 *
 * Unloads dictionary from memory.  Returns true if successful else false.
 */

bool
unload()
{
    node *slot;
    node *tmp;

    // iterate through linked lists and free each one
    for (int i = 0; i < 100000; i++) {
        slot = words[i];
        
        while (slot->next != NULL) {
            tmp = slot;
            slot = slot->next;
            free(tmp);
        }
        free(slot);
    }
    return true;
}

/*
 * unsigned long hash(unsigned char *str)
 *
 * Implements djb2 hash
 */
unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}
