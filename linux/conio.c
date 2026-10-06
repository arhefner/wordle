#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "conio.h"

int getch(void)
{
    struct termios oldt, newt;
    int ch;

    // Show any pending output before waiting for a key
    fflush(stdout);

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

int putch(int ch)
{
    return putchar(ch);
}

int cputs(const char *s)
{
    return fputs(s, stdout);
}

void clrscr(void)
{
    // Erase the screen and home the cursor
    fputs("\x1b[2J\x1b[H", stdout);
}

void gotoxy(int x, int y)
{
    printf("\x1b[%d;%dH", y, x);
}
