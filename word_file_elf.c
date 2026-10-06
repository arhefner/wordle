#include <stdlib.h>
#include <math32.h>
#include "wordle.h"

int get_word_count(int fd)
{
    off_t file_size;
    int32_t error;
    int32_t num_words;
    int32_t line_size;
    int32_t zero;
#ifndef __ELFDOS__
    char *fildes;
#endif

    zero = i32_from_int(0);

    // Get file size
    file_size = lseek32(fd, zero, SEEK_END);

    error = i32_from_int(-1);

    if (cmpi32(file_size, error) == 0) {
        return -1;
    }

#ifndef __ELFDOS__
    // Elf/OS v5 returns a sector number from a seek rather than the
    // position, so read the position from the file descriptor instead
    fildes = (char *)_fildes(fd);
    file_size.high = ((fildes[0] & 0xFF) << 8) | (fildes[1] & 0xFF);
    file_size.low = ((fildes[2] & 0xFF) << 8) | (fildes[3] & 0xFF);
#endif

    line_size = i32_from_int(NUM_LETTERS + 1);

    num_words = divi32(file_size, line_size, NULL);

    return num_words.low;
}

void get_word(int fd, int pos, char *buf)
{
    int fildes;
    int bytes_read;

    /* get system file descriptor */
    fildes = _fildes(fd);

    /* don't seek invalid fd */
    if(fildes == EOF) {
        *buf = '\0';
        return;
    }

    asm("         gosub s_lget16  ; get the fildes variable ");
    asm("           dw -2         ; from local variable stack");
    asm("         copy ra, rd     ; copy fd to fildes register");
    asm("         gosub s_lget16  ; get the position");
    asm("           dw 2          ; from argument stack");
    asm("         copy ra, rf     ; copy position to low offset");
    asm("         ldi  0          ; load zero");
    asm("         phi  rc         ; load SEEK_SET(0) into");
    asm("         plo  rc         ; how to seek register");
    asm("         phi  r8         ; set upper word of offset");
    asm("         plo  r8         ; to zero");
    asm("         glo  rf         ; mutiply offset by 2");
    asm("         shl             ; by shifting left");
    asm("         plo  rf         ; one bit");
    asm("         ghi  rf         ;");
    asm("         shlc            ;");
    asm("         phi  rf         ;");
    asm("         glo  r8         ;");
    asm("         shlc            ;");
    asm("         plo  r8         ;");
    asm("         ghi  r8         ;");
    asm("         shlc            ;");
    asm("         phi  r8         ;");
    asm("         stxd            ; save pos * 2");
    asm("         glo  r8         ; on stack");
    asm("         stxd            ; from high byte");
    asm("         ghi  rf         ;");
    asm("         stxd            ;");
    asm("         glo  rf         ;");
    asm("         stxd            ; to low byte");
    asm("         shl             ; multiply by 2 again");
    asm("         plo  rf         ; by shifting left");
    asm("         ghi  rf         ; one bit");
    asm("         shlc            ; to yield pos * 4");
    asm("         phi  rf         ;");
    asm("         glo  r8         ;");
    asm("         shlc            ;");
    asm("         plo  r8         ;");
    asm("         ghi  r8         ;");
    asm("         shlc            ;");
    asm("         phi  r8         ;");
    asm("         irx             ; point X to low byte");
    asm("         glo  rf         ; of pos * 2 on stack");
    asm("         add             ; add pos * 4 to pos * 2");
    asm("         plo  rf         ; yielding pos * 6");
    asm("         irx             ;");
    asm("         ghi  rf         ;");
    asm("         adc             ;");
    asm("         phi  rf         ;");
    asm("         irx             ;");
    asm("         glo  r8         ;");
    asm("         adc             ;");
    asm("         plo  r8         ;");
    asm("         irx             ;");
    asm("         ghi  r8         ;");
    asm("         adc             ;");
    asm("         phi  r8         ;");
#ifdef __ELFDOS__
    asm("         push r7         ; save registers used by");
    asm("         push r9         ; the compiler before");
    asm("         push rb         ; calling the kernel");
    asm("         copy rf, r9     ; set low offset for K_FILE_SEEK");
    asm("         copy r8, ra     ; set high offset for K_FILE_SEEK");
    asm("         call K_FILE_SEEK ; attempt to seek within file");
    asm("         pop  rb         ; restore registers");
    asm("         pop  r9         ;");
    asm("         pop  r7         ;");
#else
    asm("         push r7         ; save stack pointer before setting low offset");
    asm("         copy rf, r7     ; set low offset for O_SEEK");
    asm("         call O_SEEK     ; attempt to seek within file");
    asm("         pop  r7         ; restore stack pointer");
#endif

    // Read into buffer
    bytes_read = read(fd, buf, NUM_LETTERS);

    if (bytes_read < NUM_LETTERS) {
        // Return empty string on error
        buf[0] = '\0';
    }
    else {
        // Terminate the word
        buf[NUM_LETTERS] = '\0';
    }
}
