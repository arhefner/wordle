#include "wordle.h"

int getch_no_echo(void) {
  int ch;

  asm(";--------Enable raw mode");
  asm("         ghi   re            ; get Elf/OS serial byte");
  asm("         ani   $fe           ; clear the echo bit");
  asm("         phi   re            ; restore serial byte with echo off");

  asm("         call  O_READKEY     ; read a character from input");
  asm("         plo   ra            ; save in return register");
  asm("         ldi   0             ; pad register with zero");
  asm("         phi   ra            ");
  asm("         gosub s_lset16      ; set the local variable");
  asm("           dw -2             ; with the return value");

  asm(";--------Disable raw mode    ");
  asm("         ghi   re            ; get Elf/OS serial byte");
  asm("         ori   $01           ; set the echo bit");
  asm("         phi   re            ; restore serial byte with echo on");

  return ch;
}
