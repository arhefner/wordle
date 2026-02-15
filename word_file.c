#include <unistd.h>
#include "wordle.h"

int get_word_count(int fd)
{
    int file_size;
    int line_size;

    // Get file size
    file_size = lseek(fd, 0, SEEK_END);

    if (file_size <= 0) {
        return -1;
    }

    // Each line is 5 letters + newline = 6 bytes
    line_size = NUM_LETTERS + 1;

    return file_size / line_size;
}

void get_word(int fd, int pos, char *buf)
{
    int offset;
    int bytes_read;

    offset = pos * (NUM_LETTERS + 1);

    // Read the word at position mid
    lseek(fd, offset, SEEK_SET);

    // Read into buffer
    bytes_read = read(fd, buf, NUM_LETTERS);

    if (bytes_read < NUM_LETTERS) {
        // Return empty string on error
        buf[0] = '\0';
    }
    else {
        // Terminate the word
        buf[NUM_LETTERS] = '\0';
    }
}

