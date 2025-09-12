#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "convert.h"


static void demo_test_run(void) {
    char out[128];

    div_convert(156, 8, out);
    printf("Test 1: div_convert(156, 8) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
           "234", out, strcmp(out, "234") == 0 ? "PASS" : "FAIL");

    div_convert(255, 16, out);
    printf("Test 2: div_convert(255, 16) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
           "FF", out, strcmp(out, "FF") == 0 ? "PASS" : "FAIL");

    sub_convert(104, 5, out);
    printf("Test 3: sub_convert(104, 5) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
           "404", out, strcmp(out, "404") == 0 ? "PASS" : "FAIL");

    printf("Test 4: print_tables(5) -> [ FORMATTED OUTPUT CHECK ]\n");
    print_tables(5);
}


static void file_test_run(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return;
    }
    char line[512];
    int lineno = 0;
    while (fgets(line, sizeof(line), f)) {
        lineno++;
        /* trim newline */
        line[strcspn(line, "\r\n")] = '\0';
        /* skip empty or comment lines */
        char *p = line;
        while (*p == ' ' || *p == '\t') ++p;
        if (*p == '\0' || *p == '#') continue;

        /* tokenize */
        char *tok = strtok(p, " \t");
        if (!tok) continue;

        if (strcmp(tok, "div_convert") == 0 || strcmp(tok, "sub_convert") == 0) {
            int is_div = (tok[3] == ' ');
            char *numtok = strtok(NULL, " \t");
            char *basetok = strtok(NULL, " \t");
            char *expected = strtok(NULL, " \t");

            if (!numtok || !basetok) {
                printf("Line %d: malformed (expected: %s <number> <base> [expected])\n",
                       lineno, tok);
                continue;
            }
            uint32_t n = (uint32_t) strtoul(numtok, NULL, 0);
            int base = (int) strtol(basetok, NULL, 0);

            char out[256];
            if (strcmp(tok, "div_convert") == 0) div_convert(n, base, out);
            else sub_convert(n, base, out);

            if (expected) {
                /* remove surrounding quotes if present */
                size_t L = strlen(expected);
                if (L >= 2 && ((expected[0] == '"' && expected[L-1] == '"') ||
                               (expected[0] == '\'' && expected[L-1] == '\''))) {
                    expected[L-1] = '\0';
                    expected++;
                }
                printf("%s %u %d -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                       tok, n, base, expected, out, strcmp(expected, out) == 0 ? "PASS" : "FAIL");
            } else {
                printf("%s %u %d -> Got: \"%s\"\n", tok, n, base, out);
            }
        } else if (strcmp(tok, "print_tables") == 0) {
            char *numtok = strtok(NULL, " \t");
            if (!numtok) {
                printf("Line %d: malformed (expected: print_tables <number>)\n", lineno);
                continue;
            }
            uint32_t n = (uint32_t) strtoul(numtok, NULL, 0);
            printf("print_tables %u -> [ FORMATTED OUTPUT ]\n", n);
            print_tables(n);
        } else {
            printf("Line %d: unknown command: %s\n", lineno, tok);
        }
    }
    fclose(f);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("No test file provided — running built-in demo tests.\n\n");
        demo_test_run();
        return 0;
    } else {
        file_test_run(argv[1]);
        return 0;
    }
}
