#ifndef STD_SCANNER_H
#define STD_SCANNER_H

// Scanner contains input and output buffers and maintains separate indices for each
typedef struct {
    const char *in_buf;
    char *out_buf;
    size_t in_idx;
    size_t out_idx;
} Scanner;

// Initialize scanner buffers and counters
void scanner_init(Scanner *scanner);

// Return current input char and move input index up by one
char scanner_next_char(Scanner *scanner);

// Return current input char
char scanner_current_char(Scanner *scanner);

// Place char into output buf and move output index up by one
void scanner_put_char(Scanner *scanner, char c);

// Copies one scanner into another, usually for backing up and restoring indices
void scanner_copy(Scanner *scanner_src, Scanner *scanner_dst);

#endif
