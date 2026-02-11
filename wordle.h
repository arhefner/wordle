#ifndef _WORDLE_H_
#define _WORDLE_H_

#include <stdio.h>
#include <stdbool.h>

#define NUM_GUESSES         5
#define NUM_LETTERS         5

#define MAX_NAME_LEN        48
#define DEFAULT_NAME        "default"

#define HIT                 0
#define NEAR_MISS           1
#define MISS                2

extern int word_index;
extern int num_played;
extern int num_won;
extern int current_streak;
extern int max_streak;

extern char word[];
extern char guess[];
extern char match[];

extern void draw_board(void);
extern void get_guess(int index);
extern void clear_guess(int index);
extern void update_guess(int index);
extern bool replay(bool won);
extern void show_stats(void);

bool find_word(FILE* fp, const char* word);

extern int getch_no_echo(void);

extern void itoa(int num, char* str);

#endif // _WORDLE_H_
