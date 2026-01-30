#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int getch_no_echo(void)
{
    struct termios oldt, newt;
    int ch;

    // Get current terminal attributes and save them
    tcgetattr(STDIN_FILENO, &oldt);

    // Copy the settings to a new structure
    newt = oldt;

    // Disable canonical mode (ICANON) and echo (ECHO)
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply the new settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read a single character
    ch = getchar();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}
