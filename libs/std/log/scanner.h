#ifndef SCANNER_H
#define SCANNER_H

typedef struct {
    const char *in_buf;
    char *out_buf;
    size_t in_idx;
    size_t out_idx;
} Scanner;

void scanner_init(Scanner *scanner);

char scanner_next_char(Scanner *scanner);

char scanner_current_char(Scanner *scanner);

void scanner_put_char(Scanner *scanner, char c);

void scanner_copy(Scanner *scanner_src, Scanner *scanner_dst);

#endif
