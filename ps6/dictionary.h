/****************************************************************************
 * dictionary.h
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Declares a dictionary's functionality.
 ***************************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>


// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45


/*
 * bool
 * check(const char *word)
 *
 * Returns true if word is in dictionary else false.
 */

bool check(const char *word);


/*
 * bool
 * load(const char *dict)
 *
 * Loads dict into memory.  Returns true if successful else false.
 */

bool load(const char *dict);


/*
 * unsigned int
 * size()
 *
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */

unsigned int size();


/*
 * bool
 * unload()
 *
 * Unloads dictionary from memory.  Returns true if successful else false.
 */

bool unload();

// helpers
unsigned long hash(const char *str);


#endif // DICTIONARY_H
