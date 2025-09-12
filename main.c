#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "convert.h"


void run_conversion_test(const char *func, uint32_t n, int base, const char *expected) {
    char out[128] = "";
    if (strcmp(func, "div_convert") == 0) div_convert(n, base, out);
    else if (strcmp(func, "sub_convert") == 0) sub_convert(n, base, out);
    else return;

    if (expected) {
        if (strcmp(out, expected) == 0)
            printf("%s(%u, %d) -> Expected: %s, Got: %s [PASS]\n", func, n, base, expected, out);
        else
            printf("%s(%u, %d) -> Expected: %s, Got: %s [FAIL]\n", func, n, base, expected, out);
    } else {
        printf("%s(%u, %d) -> Got: %s\n", func, n, base, out);
    }
}

/* Parse and run the test file */
void run_tests_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char line[256];
    int lineno = 0;

    while (fgets(line, sizeof(line), file)) {
        lineno++;
        line[strcspn(line, "\r\n")] = '\0'; // remove newline

        if (line[0] == '\0' || line[0] == '#') continue; // skip empty/comment lines

        char *func = strtok(line, " \t");
        char *numtok = strtok(NULL, " \t");
        char *basetok = strtok(NULL, " \t");
        char *expected = strtok(NULL, " \t");

        if (!func || !numtok) continue;

        uint32_t n = (uint32_t) strtoul(numtok, NULL, 0);
        int base = basetok ? (int) strtol(basetok, NULL, 0) : 0;

        if (strcmp(func, "print_tables") == 0) {
            print_tables(n);
        } else {
            run_conversion_test(func, n, base, expected);
        }
    }

    fclose(file);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <testfile>\n", argv[0]);
        return 1;
    }

    run_tests_from_file(argv[1]);
    return 0;
}
