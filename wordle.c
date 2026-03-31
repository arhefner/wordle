#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include "wordle.h"

char name[MAX_NAME_LEN];

stat_t stats;

char word[NUM_LETTERS + 1];
char guess[NUM_LETTERS + 1];

char match[NUM_LETTERS];

char test[NUM_LETTERS + 1];

int word_count;
int answer_count;

int main(int argc, char *argv[])
{
    int words;
    int words_sorted;
    int answers;
    char *newline_pos;
    bool done;
    bool won;
    int pos;
    int i;
    int j;
    int k;
    bool valid;
    char check;

    fputs("Name? ", stdout);
    if (fgets(name, sizeof(name), stdin) == NULL) {
        strcpy(name, DEFAULT_NAME);
    }

    // Strip newline if present
    newline_pos = strchr(name, '\n');
    if (newline_pos != NULL) {
        *newline_pos = '\0';
    }

    if (!read_stats(name)) {
        memset(&stats, 0, sizeof(stats));
    }

    words = open("wordlist.txt", O_RDONLY);
    if (words == -1) {
        fputs("Could not open solution list", stdout);
        return 1;
    }

    words_sorted = open("words_sorted.txt", O_RDONLY);
    if (words_sorted == -1) {
        fputs("Could not open sorted solution list", stdout);
        return 1;
    }

    word_count = get_word_count(words_sorted);

    answers = open("answers.txt", O_RDONLY);
    if (answers == -1) {
        fputs("Could not open valid word list", stdout);
        return 1;
    }

    answer_count = get_word_count(answers);

    done = false;

    while (!done) {
        won = false;

        // Get the next word
        get_word(words, stats.word_index, word);

        draw_board();

        for (i = 0; (i < NUM_GUESSES) && !won; i++) {
            // Wait for the user to enter a valid guess
            do {
                get_guess(i);

                if (find_word(words_sorted, word_count, guess) ||
                    find_word(answers, answer_count, guess)) {
                    valid = true;
                }
                else {
                    clear_guess(i);
                    valid = false;
                }
            } while (!valid);

            strcpy(test, word);
            memset(match, MISS, NUM_LETTERS);
            won = true;

            for (j = 0; j < NUM_LETTERS; j++) {
                if (test[j] == guess[j]) {
                    match[j] = HIT;
                    test[j] = 'X';
                }
                else {
                    won = false;
                }
            }

            if (!won) {
                for (j = 0; j < NUM_LETTERS; j++) {
                    if (match[j] == MISS) {
                        for (k = 0; k < NUM_LETTERS; k++) {
                            if (test[k] == guess[j]) {
                                match[j] = NEAR_MISS;
                                test[k] = 'X';
                                break;
                            }
                        }
                    }
                }
            }

            update_guess(i);
        }

        stats.word_index++;
        stats.num_played++;

        if (won) {
            stats.num_won++;
            stats.current_streak++;
            if (stats.current_streak > stats.max_streak) {
                stats.max_streak = stats.current_streak;
            }
        }
        else {
            stats.current_streak = 0;
        }

        done = !replay(won);
    }

    close(words);
    close(words_sorted);
    close(answers);

    show_stats();

    save_stats(name);
}
