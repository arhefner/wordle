#include "wordle.h"

int get_word_count(FILE *fp)
{
    int file_size;
    int line_size;

    // Get file size
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    if (file_size <= 0) {
        return -1;
    }

    // Each line is 5 letters + newline = 6 bytes
    line_size = NUM_LETTERS + 1;
    return file_size / line_size;
}

void get_word(FILE *fp, int pos, char *buf)
{
    int offset;
    int bytes_read;

    offset = pos * (NUM_LETTERS + 1);

    // Read the word at position mid
    fseek(fp, offset, SEEK_SET);

    // Read into buffer
    bytes_read = fread(buf, 1, NUM_LETTERS, fp);

    if (bytes_read < NUM_LETTERS) {
        // Return empty string on error
        buf[0] = '\0';
    }
    else {
        // Terminate the word
        buf[NUM_LETTERS] = '\0';
    }
}