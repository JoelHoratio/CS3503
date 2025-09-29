#include "convert.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

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
        if ( rem < 10) {
          temp [pos ++] = ’0’ + rem ;
        } else {
          temp [pos ++] = ’A’ + ( rem - 10) ;}
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

        if (digit < 10) {
          out[pos++] = '0' + digit;
        }else {
          out[pos++] = 'A' + (digit - 10);}
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
    void to_32bit_binary(uint32_t value, char *out) {
    for (int i = 31; i >= 0; i--) {
        out[31 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    out[32] = '\0'; // null terminator
}
// Conversions

void oct_to_bin(const char *oct, char *out) {
    size_t len = strlen(oct);
    size_t pos = 0;

    for (size_t i = 0; i < len; i++) {
        int digit = oct[i] - '0';
        out[pos++] = (digit & 4) ? '1' : '0';
        out[pos++] = (digit & 2) ? '1' : '0';
        out[pos++] = (digit & 1) ? '1' : '0';
    }

    out[pos] = '\0';
}


void oct_to_hex(const char *oct, char *out) {
    char bin[100] = ""; // temp buffer
    oct_to_bin(oct, bin); // oct to binary

    // binary -> hex
    int len = strlen(bin);
    int padding = (4 - len % 4) % 4; // pad to multiple of 4
    char padded_bin[105];
    for (int i = 0; i < padding; i++) padded_bin[i] = '0';
    strcpy(padded_bin + padding, bin);

    int hex_len = (len + padding + 3) / 4;
    int pos = 0;
    for (int i = 0; i < hex_len; i++) {
        int index = i * 4;
        int value = 0;
        for (int j = 0; j < 4; j++) {
            value = (value << 1) + (padded_bin[index + j] - '0');
        }
        if (value < 10)
            out[pos++] = '0' + value;
        else
            out[pos++] = 'A' + (value - 10);
    }
    out[pos] = '\0';

    // remove leading zeros
    char *ptr = out;
    while (*ptr == '0' && *(ptr+1) != '\0') ptr++;
    memmove(out, ptr, strlen(ptr)+1);
}


void hex_to_bin(const char *hex, char *out) {
    size_t len = strlen(hex);
    size_t pos = 0;
    for (size_t i = 0; i < len; i++) {
        char c = hex[i];
        int val = 0;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;

        out[pos++] = (val & 8) ? '1' : '0';
        out[pos++] = (val & 4) ? '1' : '0';
        out[pos++] = (val & 2) ? '1' : '0';
        out[pos++] = (val & 1) ? '1' : '0';
    }
    out[pos] = '\0';
}

// Signed numbers

void to_sign_magnitude(int32_t n, char *out) {
    uint32_t val = (n < 0) ? -n : n;
    for (int i = 30; i >= 0; i--) {
        out[30 - i + 1] = (val & (1U << i)) ? '1' : '0';
    }
    out[0] = (n < 0) ? '1' : '0';
    out[32] = '\0';
}

void to_ones_complement(int32_t n, char *out) {
    uint32_t u;
    if (n >= 0) {
        u = (uint32_t)n;
    } else {
        u = ~((uint32_t)(-n)); // flipping all bits
    }
    to_32bit_binary(u, out);
}

void to_twos_complement(int32_t n, char *out) {
    uint32_t val = (uint32_t)n;
    for (int i = 31; i >= 0; i--) {
        out[31 - i] = (val & (1U << i)) ? '1' : '0';
    }
    out[32] = '\0';
}
}
