#ifndef _CONIO_
#define _CONIO_

/*
 * conio.h - minimal linux version of the ElfC console I/O library
 *
 * Only the functions used by wordle are provided.  The screen is
 * controlled with standard ANSI (ECMA-48) escape sequences.
 */

/* character input and output */
int getch(void);
int putch(int ch);
int cputs(const char *s);

/* screen and cursor control */
void clrscr(void);
void gotoxy(int x, int y);

#endif
