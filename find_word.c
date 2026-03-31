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
bool find_word(int fd, int word_count, const char* word)
{
    static char test[NUM_LETTERS + 1];
    int num_words;
    int left;
    int right;
    int mid;
    int cmp;

    // Binary search
    left = 0;
    right = word_count - 1;

    while (left <= right) {
        mid = left + (right - left) / 2;

        get_word(fd, mid, test);

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
