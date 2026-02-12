#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "wordle.h"

char name[MAX_NAME_LEN];

stat_t stats;

char word[NUM_LETTERS + 1];
char guess[NUM_LETTERS + 1];

char match[NUM_LETTERS];

char test[NUM_LETTERS + 1];

int main(int argc, char *argv[])
{
    FILE *words;
    FILE *words_sorted;
    FILE *answers_a_l;
    FILE *answers_m_z;
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

    words = fopen("wordlist.txt", "r");
    if (words == NULL) {
        perror("Could not open solution list");
        return 1;
    }

    words_sorted = fopen("words_sorted.txt", "r");
    if (words_sorted == NULL) {
        perror("Could not open sorted solution list");
        return 1;
    }

    answers_a_l = fopen("answers_a-l.txt", "r");
    if (answers_a_l == NULL) {
        perror("Could not open valid word list");
        return 1;
    }

    answers_m_z = fopen("answers_m-z.txt", "r");
    if (answers_m_z == NULL) {
        perror("Could not open valid word list");
        return 1;
    }

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

                if (find_word(words_sorted, guess) ||
                    ((guess[0] < 'm') && find_word(answers_a_l, guess)) ||
                    find_word(answers_m_z, guess)) {
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

    fclose(words);
    fclose(words_sorted);
    fclose(answers_a_l);
    fclose(answers_m_z);

    show_stats();

    save_stats(name);
}
