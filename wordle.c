#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "wordle.h"

char name[MAX_NAME_LEN];

int word_index;
int num_played;
int num_won;
int current_streak;
int max_streak;

char word[NUM_LETTERS + 1];
char guess[NUM_LETTERS + 1];

char map[26];
char match[NUM_LETTERS];

static FILE *open_stat_file(char *name, char *mode)
{
    static char filename[FILENAME_MAX];

    // Use "default" if name is empty
    if (name[0] == '\0') {
        strcpy(filename, "default.wdl");
    } else {
        // Append .dat extension
        strcpy(filename, name);
        strcat(filename, ".wdl");
    }

    // Open the file
    return fopen(filename, mode);
}

bool read_stats(char *name)
{
    FILE *fp;
    static char line[64];
    char *ptr;

    // Open stat file for name
    fp = open_stat_file(name, "r");

    // If no stat file was found, return
    if (fp == NULL) {
        return false;
    }

    // Read a line from the file
    if (fgets(line, sizeof(line), fp) == NULL) {
        return false;  // End of file or error
    }

    word_index = atoi(line);

    ptr = strchr(line, ',');
    if (ptr == NULL) {
        return false;
    }

    num_played = atoi(ptr + 1);

    ptr = strchr(ptr + 1, ',');
    if (ptr == NULL) {
        return false;
    }

    num_won = atoi(ptr + 1);

    ptr = strchr(ptr + 1, ',');
    if (ptr == NULL) {
        return false;
    }

    current_streak = atoi(ptr + 1);

    ptr = strchr(ptr + 1, ',');
    if (ptr == NULL) {
        return false;
    }

    max_streak = atoi(ptr + 1);

    fclose(fp);

    return true;  // Success
}

static void save_stats(char *name)
{
    FILE *fp;
    char buffer[10];

    fp = open_stat_file(name, "w");

    if (fp == NULL) {
        return;
    }

    itoa(word_index, buffer);
    fputs(buffer, fp);
    fputc(',', fp);

    itoa(num_played, buffer);
    fputs(buffer, fp);
    fputc(',', fp);

    itoa(num_won, buffer);
    fputs(buffer, fp);
    fputc(',', fp);

    itoa(current_streak, buffer);
    fputs(buffer, fp);
    fputc(',', fp);

    itoa(max_streak, buffer);
    fputs(buffer, fp);

    fputc('\n', fp);

    fclose(fp);
}

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
        word_index = 0;
        num_played = 0;
        num_won = 0;
        current_streak = 0;
        max_streak = 0;
    }

    words = fopen("wordlist.txt", "r");
    if (word == NULL) {
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
    if (answers_a_l == NULL) {
        perror("Could not open valid word list");
        return 1;
    }

    done = false;

    while (!done) {
        won = false;

        // Get the next word
        pos = word_index * (NUM_LETTERS + 1);
        fseek(words, pos, SEEK_SET);
        fgets(word, sizeof(word), words);

        memset(map, 0, sizeof(map));
        pos = 1;
        for (i = 0; i < NUM_LETTERS; i++) {
            map[word[i] - 'a'] |= pos;
            pos <<= 1;
        }

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

            if (strcmp(guess, word) == 0) {
                memset(match, HIT, sizeof(match));
                won = true;
            }
            else {
                pos = 1;
                for (j = 0; j < NUM_LETTERS; j++) {
                    check = map[guess[j] - 'a'];
                    if (check == 0) {
                        match[j] = MISS;
                    }
                    else if ((check & pos) != 0) {
                        match[j] = HIT;
                    }
                    else {
                        match[j] = NEAR_MISS;
                    }
                    pos <<= 1;
                }
            }

            update_guess(i);
        }

        word_index++;
        num_played++;

        if (won) {
            num_won++;
            current_streak++;
            if (current_streak > max_streak) {
                max_streak = current_streak;
            }
        }
        else {
            current_streak = 0;
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
