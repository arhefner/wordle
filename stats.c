#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#ifdef __linux__
#include <sys/stat.h>
#else
#define S_IRUSR     0
#define S_IWUSR     0
#endif
#include "wordle.h"

static int open_stat_file(char *name, int oflag, int mode)
{
    static char filename[FILENAME_MAX];
    int fd;

    // Use "default" if name is empty
    if (name[0] == '\0') {
        strcpy(filename, "default.wdl");
    } else {
        // Append .dat extension
        strcpy(filename, name);
        strcat(filename, ".wdl");
    }

    // Open the file
#ifdef __linux__
    fd = open(filename, oflag, mode);
#else
    fd = open(filename, oflag);
#endif

    return fd;
}

bool read_stats(char *name)
{
    int fd;

    // Open stat file for name
    fd = open_stat_file(name, O_RDONLY, 0);

    // If no stat file was found, return
    if (fd == -1) {
        return false;
    }

    // Read the stat structure from the file
    if (read(fd, &stats, sizeof(stats)) != sizeof(stats)) {
        return false;
    }

    close(fd);

    return true;  // Success
}

void save_stats(char *name)
{
    int fd;

    fd = open_stat_file(name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        return;
    }

    write(fd, &stats, sizeof(stats));

    close(fd);
}
