# Build wordle for linux, Elf/OS or ELF-DOS
#
#   make [linux|elfos|elfdos|all] [BOARD=unibox|vtbox|ascii]
#
# Output is written to bin/<target>/

BOARD ?= unibox
CC    ?= gcc
# Resolved by the shell, as make can mistake a folder named elfc on the
# PATH for the compiler
ELFC  ?= $(or $(shell command -v elfc),elfc)

ifeq ($(BOARD),ascii)
BOARD_SRC = board.c
else ifneq ($(filter $(BOARD),unibox vtbox),)
BOARD_SRC = board_$(BOARD).c
else
$(error BOARD must be unibox, vtbox or ascii)
endif

COMMON_SRC = wordle.c $(BOARD_SRC) find_word.c stats.c
LINUX_SRC  = $(COMMON_SRC) itoa.c word_file.c linux/conio.c
ELF_SRC    = $(COMMON_SRC) word_file_elf.c

.PHONY: linux elfos elfdos all clean

linux:
	mkdir -p bin/linux
	$(CC) $(CFLAGS) -Ilinux -o bin/linux/wordle $(LINUX_SRC)

elfos:
	mkdir -p bin/elfos
	$(ELFC) -o bin/elfos/wordle.elfos $(ELF_SRC)

elfdos:
	mkdir -p bin/elfdos
	$(ELFC) -E -o bin/elfdos/wordle $(ELF_SRC)

all: linux elfos elfdos

ELF_NAMES = wordle board board_unibox board_vtbox find_word stats word_file_elf

clean:
	rm -rf bin
	rm -f $(foreach ext,asm prg lst build,$(addsuffix .$(ext),$(ELF_NAMES)))
