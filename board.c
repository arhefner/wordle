#include <ctype.h>
#include <conio.h>
#include "wordle.h"

#define ROW_ORIGIN      2
#define COL_ORIGIN      3
#define ROW_NEXT        2
#define COL_NEXT        4

#define ESC             '\x1b'
#define HOME            "\x1b[H"

#define NORMAL          "\x1b[0m"
#define BOLD            "\x1b[1m"
#define UNDERLINE       "\x1b[4m"
#define REVERSE         "\x1b[7m"

static int row;
static int col;
static int guess_col;

static void draw_line(void)
{
    int i;

    putch('+');
    for (i = 0; i < NUM_LETTERS; i++) {
        cputs("---+");
    }
    cputs("\r\n");
}

static void draw_row(void)
{
    int i;

    putch('|');
    for (i = 0; i < NUM_LETTERS; i++) {
        cputs("   |");
    }
    cputs("\r\n");
}

void draw_board(void)
{
    int i;
    clrscr();
    cputs(HOME);

    draw_line();
    for (i = 0; i < NUM_GUESSES; i++) {
        draw_row();
        draw_line();
    }
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

    cputs(NORMAL);

    guess_col = 0;
    done = false;
    escape = false;

    while (!done) {
        gotoxy(col, row);

        ch = getch();

        if (escape) {
            if ((ch != '[') && (ch != 'O')) {
                if (ch == 'C') {
                    // right arrow
                    if (guess_col < (NUM_LETTERS - 1)) {
                        guess_col++;
                        col += COL_NEXT;
                        gotoxy(col, row);
                    }
                }
                else if (ch == 'D') {
                    // left arrow
                    if (guess_col > 0) {
                        guess_col--;
                        col -= COL_NEXT;
                        gotoxy(col, row);
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
                gotoxy(col, row);
                putch(' ');
                gotoxy(col, row);
            }
        }
        else {
            if (isalpha(ch) && (guess_col < NUM_LETTERS)) {
                putch(toupper(ch));
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

    cputs(NORMAL);

    for (i = 0; i < NUM_LETTERS; i++) {
        guess[i] = ' ';
        gotoxy(col, row);
        putch(' ');
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
        gotoxy(col - 1, row);
        if (match[i] == HIT) {
            cputs(REVERSE);
        }
        else if (match[i] == NEAR_MISS) {
            cputs(UNDERLINE);
        }
        putch(' ');
        putch(toupper(guess[i]));
        putch(' ');
        cputs(NORMAL);
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
    gotoxy(col, row);

    if (won) {
        cputs("Congratulations!\r\n");
    }
    else {
        cputs("Sorry, it was '");
        cputs(word);
        cputs("'.\r\n");
    }

    cputs("Play again (Y/N)? ");
    ch = getch();
    return (toupper(ch) == 'Y');
}

void show_stats(void)
{
    char buffer[10];
    int percentage;

    clrscr();
    cputs(HOME);

    cputs("Games played: ");
    itoa(stats.num_played, buffer);
    cputs(buffer);
    cputs("\r\n");

    cputs("Games won: ");
    itoa(stats.num_won, buffer);
    cputs(buffer);
    cputs("\r\n");

    percentage = (stats.num_won * 100) / stats.num_played;

    cputs("Win Percentage: ");
    itoa(percentage, buffer);
    cputs(buffer);
    cputs("%\r\n");

    cputs("Current Streak: ");
    itoa(stats.current_streak, buffer);
    cputs(buffer);
    cputs("\r\n");

    cputs("Max Streak: ");
    itoa(stats.max_streak, buffer);
    cputs(buffer);
    cputs("\r\n");
}
