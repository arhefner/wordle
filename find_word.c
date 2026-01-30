#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define WORD_LENGTH 5

/**
 * Searches for a 5-letter word in a sorted file using binary search.
 *
 * @param fp File pointer to a file containing sorted 5-letter words (one per line)
 * @param word Pointer to a 5-letter word to search for
 * @return true if the word is found, false otherwise
 */
bool find_word(FILE* fp, const char* word)
{
    int file_size;
    int line_size;
    int num_words;
    int left;
    int right;
    int mid;
    int offset;
    char buffer[WORD_LENGTH + 1];
    size_t bytes_read;
    int cmp;

    if (fp == NULL || word == NULL) {
        return false;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    if (file_size <= 0) {
        return false;
    }

    // Each line is 5 letters + newline = 6 bytes
    line_size = WORD_LENGTH + 1;
    num_words = file_size / line_size;

    // Binary search
    left = 0;
    right = num_words - 1;
    buffer[WORD_LENGTH + 1];

    while (left <= right) {
        mid = left + (right - left) / 2;
        offset = mid * line_size;

        // Read the word at position mid
        fseek(fp, offset, SEEK_SET);

        // Read into buffer
        bytes_read = fread(buffer, 1, WORD_LENGTH, fp);

        if (bytes_read < WORD_LENGTH) {
            return false;
        }

        // Null-terminate for comparison
        buffer[WORD_LENGTH] = '\0';

        // Compare
        cmp = strncmp(word, buffer, WORD_LENGTH);

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
