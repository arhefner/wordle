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

typedef struct {
    int word_index;
    int num_played;
    int num_won;
    int current_streak;
    int max_streak;
} stat_t;

extern stat_t stats;

extern char word[];
extern char guess[];
extern char match[];

extern bool read_stats(char *name);
extern void save_stats(char *name);

extern int get_word_count(FILE *file);
extern void get_word(FILE *file, int pos, char *buf);

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
