#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "wordle.h"

/**
 * Searches for a 5-letter word in a sorted file using binary search.
 *
 * @param fp File pointer to a file containing sorted 5-letter words (one per line)
 * @param word Pointer to a 5-letter word to search for
 * @return true if the word is found, false otherwise
 */
bool find_word(FILE* fp, const char* word)
{
    static char test[NUM_LETTERS + 1];
    int num_words;
    int left;
    int right;
    int mid;
    int cmp;

    if (fp == NULL || word == NULL) {
        return false;
    }

    // Get word count
    num_words = get_word_count(fp);

    // Binary search
    left = 0;
    right = num_words - 1;

    while (left <= right) {
        mid = left + (right - left) / 2;

        get_word(fp, mid, test);

        // Compare
        cmp = strncmp(word, test, NUM_LETTERS);

        if (cmp == 0) {
            return true;  // Found it!
        } else if (cmp < 0) {
            right = mid - 1;  // Search left half
        } else {
            left = mid + 1;   // Search right half
        }
    }

    return false;  // Not found
}
