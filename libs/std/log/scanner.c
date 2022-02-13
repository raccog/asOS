#include <std/log/scanner.h>

void scanner_init(Scanner *scanner) {
    scanner->in_buf = 0;
    scanner->out_buf = 0;
    scanner->in_idx = 0;
    scanner->out_idx = 0;
}

char scanner_next_char(Scanner *scanner) {
    char c = scanner->in_buf[scanner->in_idx];
    ++(scanner->in_idx);
    return c;
}

char scanner_current_char(Scanner *scanner) {
    return scanner->in_buf[scanner->in_idx];
}

void scanner_put_char(Scanner *scanner, char c) {
    scanner->out_buf[scanner->out_idx] = c;
    ++(scanner->out_idx);
}

void scanner_copy(Scanner *scanner_src, Scanner *scanner_dst) {
    scanner_dst->in_buf = scanner_src->in_buf;
    scanner_dst->out_buf = scanner_src->out_buf;
    scanner_dst->in_idx = scanner_src->in_idx;
    scanner_dst->out_idx = scanner_src->out_idx;
}
