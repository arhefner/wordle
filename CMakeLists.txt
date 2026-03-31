cmake_minimum_required(VERSION 3.12)
project(wordle C)

add_executable(wordle wordle.c board_unibox.c find_word.c itoa.c stats.c word_file.c getch.c)

