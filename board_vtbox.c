#include <ctype.h>
#include "wordle.h"

#define ROW_ORIGIN      2
#define COL_ORIGIN      3
#define ROW_NEXT        2
#define COL_NEXT        4

#define ESC             '\x1b'
#define CLEAR_SCREEN    "\x1b[2J"
#define HOME            "\x1b[H"

#define NORMAL          "\x1b[0m"
#define BOLD            "\x1b[1m"
#define UNDERLINE       "\x1b[4m"
#define BLINK           "\x1b[5m"
#define REVERSE         "\x1b[7m"

static int row;
static int col;
static int guess_col;

static void set_cursor(int row, int col)
{
    static char buffer[5];

    fputs("\x1b[", stdout);
    itoa(row, buffer);
    fputs(buffer, stdout);
    fputc(';', stdout);
    itoa(col, buffer);
    fputs(buffer, stdout);
    fputc('H', stdout);
}

static void draw_line(char start, char middle, char end)
{
    int i;

    fputc(start, stdout);
    for (i = 0; i < (NUM_LETTERS - 1); i++) {
        fputs("\x71\x71\x71", stdout);
        fputc(middle, stdout);
    }
    fputs("\x71\x71\x71", stdout);
    fputc(end, stdout);
    fputs("\r\n", stdout);
}

static void draw_row(void)
{
    int i;

    fputc('\x78', stdout);
    for (i = 0; i < NUM_LETTERS; i++) {
        fputs("   \x78", stdout);
    }
    fputs("\r\n", stdout);
}

void draw_board(void)
{
    int i;
    fputs(CLEAR_SCREEN, stdout);
    fputs(HOME, stdout);

    fputs("\x1b(B", stdout);
    fputs("\x1b)0", stdout);
    fputc('\x0e', stdout);

    draw_line('\x6c', '\x77', '\x6b');
    for (i = 0; i < (NUM_GUESSES - 1); i++) {
        draw_row();
        draw_line('\x74', '\x6e', '\x75');
    }
    draw_row();
    draw_line('\x6d', '\x76', '\x6a');

    fputc('\x0f', stdout);
}

void get_guess(int index)
{
    int i;
    int ch;
    bool done;
    bool escape;

    row = ROW_ORIGIN;
    col = COL_ORIGIN;

    for (i = 0; i < index; i++) {
        row += ROW_NEXT;
    }

    guess[NUM_LETTERS] = '\0';

    fputs(NORMAL, stdout);

    guess_col = 0;
    done = false;
    escape = false;

    while (!done) {
        set_cursor(row, col);

        ch = getch_no_echo();

        if (escape) {
            if ((ch != '[') && (ch != 'O')) {
                if (ch == 'C') {
                    // right arrow
                    if (guess_col < (NUM_LETTERS - 1)) {
                        guess_col++;
                        col += COL_NEXT;
                        set_cursor(row, col);
                    }
                }
                else if (ch == 'D') {
                    // left arrow
                    if (guess_col > 0) {
                        guess_col--;
                        col -= COL_NEXT;
                        set_cursor(row, col);
                    }
                }

                escape = false;
            }
        }
        else if (ch == ESC) {
            escape = true;
        }
        else if ((ch == '\r') || (ch == '\n')) {
            done = true;
        }
        else if (ch == '\b' || ch == '\x1f' || ch == '\x7f') {
            if (guess_col > 0) {
                guess_col--;
                col -= COL_NEXT;
                set_cursor(row, col);
                fputc(' ', stdout);
                set_cursor(row, col);
            }
        }
        else {
            if (isalpha(ch) && (guess_col < NUM_LETTERS)) {
                fputc(toupper(ch), stdout);
                guess[guess_col++] = tolower(ch);
                col += COL_NEXT;
            }
        }
    }
}

void clear_guess(int index)
{
    int i;

    row = ROW_ORIGIN;
    col = COL_ORIGIN;

    for (i = 0; i < index; i++) {
        row += ROW_NEXT;
    }

    fputs(NORMAL, stdout);

    for (i = 0; i < NUM_LETTERS; i++) {
        guess[i] = ' ';
        set_cursor(row, col);
        fputc(' ', stdout);
        col += COL_NEXT;
    }
}

void update_guess(int index)
{
    int row = ROW_ORIGIN;
    int col = COL_ORIGIN;
    int i;

    for (i = 0; i < index; i++) {
        row += ROW_NEXT;
    }

    for (i = 0; i < NUM_LETTERS; i++) {
        set_cursor(row, col - 1);
        if (match[i] == HIT) {
            fputs(REVERSE, stdout);
        }
        else if (match[i] == NEAR_MISS) {
            fputs(UNDERLINE, stdout);
        }
        fputc(' ', stdout);
        fputc(toupper(guess[i]), stdout);
        fputc(' ', stdout);
        fputs(NORMAL, stdout);
        col += COL_NEXT;
    }
}

bool replay(bool won)
{
    int i;
    int ch;

    row = ROW_ORIGIN;

    for (i = 0; i < NUM_GUESSES; i++) {
        row += ROW_NEXT;
    }

    col = 0;
    set_cursor(row, col);

    if (won) {
        fputs("Congratulations!\r\n", stdout);
    }
    else {
        fputs("Sorry, it was '", stdout);
        fputs(word, stdout);
        fputs("'.\r\n", stdout);
    }

    fputs("Play again (Y/N)? ", stdout);
    ch = getch_no_echo();
    return (toupper(ch) == 'Y');
}

void show_stats(void)
{
    char buffer[10];
    int percentage;

    fputs(CLEAR_SCREEN, stdout);
    fputs(HOME, stdout);

    fputs("Games played: ", stdout);
    itoa(num_played, buffer);
    fputs(buffer, stdout);
    fputs("\r\n", stdout);

    fputs("Games won: ", stdout);
    itoa(num_won, buffer);
    fputs(buffer, stdout);
    fputs("\r\n", stdout);

    percentage = (num_won * 100) / num_played;

    fputs("Win Percentage: ", stdout);
    itoa(percentage, buffer);
    fputs(buffer, stdout);
    fputs("%\r\n", stdout);

    fputs("Current Streak: ", stdout);
    itoa(current_streak, buffer);
    fputs(buffer, stdout);
    fputs("\r\n", stdout);

    fputs("Max Streak: ", stdout);
    itoa(max_streak, buffer);
    fputs(buffer, stdout);
    fputs("\r\n", stdout);
}
