# wordle
## Overview
The original wordle was a web-based word game written by Josh Wardle (https://en.wikipedia.org/wiki/Wordle).
This is an implementation of a similar game written in C. This version was created to be used with the ELFC C compiler for RCA CDP1802 based computers hosted [here](https://github.com/fourstix/ELFC). But it can also be built for most linux-based systems, including WSL on Windows and the Raspberry Pi.
## Building with ELFC for Elf/OS
The program may be built with ELFC using the following command:
`elfc -o wordle.elfos wordle.c board.c find_word.c getch_elfos.c`
## Building with gcc for linux
The program may be built using CMake with the following commands:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
Alternatively, it can be built with gcc using the following command: `gcc -o wordle wordle.c board_unibox.c find_word.c itoa.c stats.c word_file.c getch.c`
## Running the program
### Elf/OS
1. Transfer the wordle.elfos file to a folder on elfos. You may simply name it wordle on the Elf/OS system.
2. Transfer the data files (the four files with a `.txt` extension) to the same folder.
3. Make sure terminal echo is on: `echoon`
4. Make sure the file is executable: `chmod +x wordle`
5. Make sure the folder is your current working directory, and execute the program.
### linux
Place the executable and the four data files in the same folder, change to that folder, and run the executable.
## Playing the game
The game will start by prompting for your name. The name is used as the name of a data file (with a `.wdl` extension) that will hold your progress and statistics about the game. In this way multiple people may progress through the game at their own pace and keep track of their progress.

The screen will clear and ablank grid of 5 lines of 5 characters will appear. The next word will be read from the 
wordlist.txt data file. Enter a guess on each row, and the letters will be marked as follows:
- If the letter is present in the word at that position, it will be shown in reverse video.
- If the letter is present in the word but in a different position, it will be underlined.
- If the letter does not occur in the word, it will be left unmarked.

You have five chances to determine the word. The program will display the outcome and give you the opportunity to play again. If you decline, your current statistics will be displayed and the game exits.
## Variations
### Number of guesses
The nuber of guesses is defined by the `NUM_GUESSES` constant in wordle.h. The default is five; the game can be made easier by increasing the number of guesses.
### Boards
The display portion of the code is located in the `board.c` file. There are currently 3 options:

1. `board.c` This is a simple ASCII interface that should work on any terminal. It gives the game a retro feel from when terminals were simple text display devices.

2. `board_vtbox.c` This file uses the DEC Special Graphics character set from the VT100/VT102 terminals to draw the game grid. It requires a terminal or terminal emulator that includes this feature. I have determined that it works with picocom, xterm, and Windows terminal. It does not work on minicom, even if it is set for VT102 emulation. It should also work on a vintage VT100/VT102 terminal if you have one.

3. `board_unibox.c` This file uses UTF-8 Unicode characters to draw the grid. It seems to be the most widely supported, as it has worked on every modern terminal program I have tried.

Simply substute the desired board file in the compile command above.

The intent is that other interfaces, perhaps using an OLED display or other graphical device may be added in the future.
