# wordle
## Overview
The original wordle was a web-based word game written by Josh Wardle (https://en.wikipedia.org/wiki/Wordle).
This is an implementation of a similar game written in C. This version was created to be used with the ELFC C compiler for RCA CDP1802 based computers hosted [here](https://github.com/fourstix/ELFC). But it can also be built for most linux-based systems, including WSL on Windows and the Raspberry Pi.
## Building
The included Makefile builds the program for any of the three systems. The output is written to a folder under `bin`:

| Command | System | Output |
| --- | --- | --- |
| `make linux` | linux (gcc) | `bin/linux/wordle` |
| `make elfos` | Elf/OS (ELFC) | `bin/elfos/wordle.elfos` |
| `make elfdos` | ELF-DOS (ELFC) | `bin/elfdos/wordle` |
| `make all` | all three | |

`make` with no target builds for linux. `make clean` removes the output and the intermediate files ELFC leaves in the source folder.

The board style is chosen with the `BOARD` variable, which may be `unibox` (the default), `vtbox` or `ascii`. See [Boards](#boards) below. For example, to build for ELF-DOS with the VT100 board: `make elfdos BOARD=vtbox`

The Elf/OS and ELF-DOS builds require the `elfc` command to be on the `PATH`, or its location may be given with `make elfos ELFC=/path/to/elfc`.
### Building without make
The Makefile runs the following commands, shown here for the Unicode board:
- linux: `gcc -Ilinux -o wordle wordle.c board_unibox.c find_word.c stats.c itoa.c word_file.c linux/conio.c`
- Elf/OS: `elfc wordle.c board_unibox.c find_word.c stats.c word_file_elf.c`
- ELF-DOS: `elfc -E wordle.c board_unibox.c find_word.c stats.c word_file_elf.c`

The `linux` folder holds a minimal version of the ElfC `conio.h` library, with only the functions this program uses.
## Running the program
### Elf/OS
On Elf/OS v5, kernel build 229 (version 5.1.1) or later is required. Earlier v5 kernels return the wrong value from `O_SEEK`, which causes every guess to be rejected as not a valid word.

1. Transfer the wordle.elfos file to a folder on elfos. You may simply name it wordle on the Elf/OS system.
2. Transfer the data files (the three files with a `.txt` extension) to the same folder.
3. Make sure terminal echo is on: `echoon`
4. Make sure the file is executable: `chmod +x wordle`
5. Make sure the folder is your current working directory, and execute the program.
### linux
Place the executable and the three data files in the same folder, change to that folder, and run the executable.
## Playing the game
The game will start by prompting for your name. The name is used as the name of a data file (with a `.wdl` extension) that will hold your progress and statistics about the game. In this way multiple people may progress through the game at their own pace and keep track of their progress.

The screen will clear and a blank grid of 6 lines of 5 characters will appear. The next word will be read from the 
wordlist.txt data file. Enter a guess on each row, and the letters will be marked as follows:
- If the letter is present in the word at that position, it will be shown in reverse video.
- If the letter is present in the word but in a different position, it will be underlined.
- If the letter does not occur in the word, it will be left unmarked.

You have six chances to determine the word. The program will display the outcome and give you the opportunity to play again. If you decline, your current statistics will be displayed and the game exits.
## Variations
### Number of guesses
The number of guesses is defined by the `NUM_GUESSES` constant in wordle.h. The default is six; the game can be made easier by increasing the number of guesses.
### Boards
The display portion of the code is located in a board file. There are currently 3 options, selected with the `BOARD` variable when building:

1. `ascii` (`board.c`) This is a simple ASCII interface that should work on any terminal. It gives the game a retro feel from when terminals were simple text display devices.

2. `vtbox` (`board_vtbox.c`) This file uses the DEC Special Graphics character set from the VT100/VT102 terminals to draw the game grid. It requires a terminal or terminal emulator that includes this feature. I have determined that it works with picocom, xterm, and Windows terminal. It does not work on minicom, even if it is set for VT102 emulation. It should also work on a vintage VT100/VT102 terminal if you have one.

3. `unibox` (`board_unibox.c`) This file uses UTF-8 Unicode characters to draw the grid. It seems to be the most widely supported, as it has worked on every modern terminal program I have tried.

The intent is that other interfaces, perhaps using an OLED display or other graphical device may be added in the future.
