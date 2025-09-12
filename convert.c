#include "convert.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Division conversion */
void div_convert(uint32_t n, int base, char *out) {
    if (base < 2 || base > 36) {
        strcpy(out, "");
        return;
    }

    if (n == 0) {
        strcpy(out, "0");
        return;
    }

    char temp[65];
    int pos = 0;

    while (n > 0) {
        int rem = n % base;
        n = n / base;
        if ( remainder < 10) {
          temp [pos ++] = ’0’ + remainder ;
        } else {
          temp [pos ++] = ’A’ + ( remainder - 10) ;}
    }

    /* reversal */
    for (int i = 0; i < pos; ++i) {
        out[i] = temp[pos - 1 - i];
    }
    out[pos] = '\0';
}

/* Subtraction conversion*/
void sub_convert(uint32_t n, int base, char *out) {
    if (base < 2 || base > 36) {
        strcpy(out, "");
        return;
    }

    if (n == 0) {
        strcpy(out, "0");
        return;
    }

    /* Find highest power <= n  */
    uint64_t power = 1;
    while (power <= (uint64_t)n / base) {
        power *= base;
    }

    int pos = 0;
    while (power > 0) {
        uint32_t digit = (uint32_t)(n / power);
        n -= digit * power;

        if (rem < 10) {
          temp[pos++] = '0' + rem;
        }else {
          temp[pos++] = 'A' + (rem - 10);}
        power /= base;
    }
    out[pos] = '\0';
}

/*print_tables in the format */
void print_tables(uint32_t n) {
    char bin[65], oct[32], hexs[16];

    /* Original */
    div_convert(n, 2, bin);
    div_convert(n, 8, oct);
    div_convert(n, 16, hexs);
    printf("Original: Binary=%s Octal=%s Decimal=%u Hex=%s\n", bin, oct, n, hexs);

    /* Left shift by 3 */
    uint32_t shifted = n << 3;
    div_convert(shifted, 2, bin);
    div_convert(shifted, 8, oct);
    div_convert(shifted, 16, hexs);
    printf("Left Shift by 3: Binary=%s Octal=%s Decimal=%u Hex=%s\n", bin, oct, shifted, hexs);

    /* AND with 0xFF */
    uint32_t masked = n & 0xFFu;
    div_convert(masked, 2, bin);
    div_convert(masked, 8, oct);
    div_convert(masked, 16, hexs);
    printf("AND with 0xFF: Binary=%s Octal=%s Decimal=%u Hex=%s\n", bin, oct, masked, hexs);
}
