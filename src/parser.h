#pragma once

struct parser_s {
    int cs;
    int number;
};

int parser_init (
    struct parser_s * parser
);

int parser_parse (
    struct parser_s * parser,
    char * buf,
    int buf_len
);
