
#include <stdbool.h>

static void reverse(char str[], int length)
{
    int start;
    int end;
    char temp;

    start = 0;
    end = length - 1;
    while (start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

void itoa(int num, char *str)
{
    int i = 0;
    bool is_negative = false;
    int rem;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        rem = num % 10;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / 10;
    }

    // If number is negative, append '-'
    if (is_negative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return;
}
