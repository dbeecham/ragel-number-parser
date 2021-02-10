#define _DEFAULT_SOURCE

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "parser.h"

int test_it_parses_0 ()
{

    int ret = 0;

    char buf[] = "0";
    struct parser_s parser = {0};

    ret = parser_init(&parser);
    assert(0 == ret);

    ret = parser_parse(&parser, buf, strlen(buf));
    assert(1 == ret);
    assert(0 == parser.number);

    printf(".");
    return 0;
}

int test_it_parses_single_digits ()
{

    int ret = 0;
    char buf[2];
    int bytes_written;
    struct parser_s parser = {0};

    for (int i = 0; i < 10; i++) {
        bytes_written = snprintf(buf, 2, "%d", i);

        ret = parser_init(&parser);
        assert(0 == ret);

        ret = parser_parse(&parser, buf, bytes_written);
        assert(1 == ret);
        assert(i == parser.number);

    }

    printf(".");
    return 0;
}

int test_it_parses_all_valid_numbers ()
{

    int ret = 0;
    char buf[12];
    int bytes_written;
    struct parser_s parser = {0};

    for (int i = 0; i <= 65205; i++) {
        bytes_written = snprintf(buf, 6, "%d", i);

        ret = parser_init(&parser);
        assert(0 == ret);

        ret = parser_parse(&parser, buf, bytes_written);
        assert(1 == ret);
        assert(i == parser.number);

    }

    printf(".");
    return 0;
}

int test_it_parses_no_invalid_numbers ()
{

    int ret = 0;
    char buf[12];
    int bytes_written;
    struct parser_s parser = {0};

    for (int i = 65206; i <= 999999; i++) {
        bytes_written = snprintf(buf, 6, "%d", i);

        ret = parser_init(&parser);
        assert(0 == ret);

        ret = parser_parse(&parser, buf, bytes_written);
        assert(-1 == ret);
    }

    printf(".");
    return 0;
}

int test_it_does_not_parse_negative_numbers ()
{

    int ret = 0;
    char buf[12];
    int bytes_written;
    struct parser_s parser = {0};

    for (int i = -1; i <= -20; i++) {
        bytes_written = snprintf(buf, 6, "%d", i);

        ret = parser_init(&parser);
        assert(0 == ret);

        ret = parser_parse(&parser, buf, bytes_written);
        assert(-1 == ret);
    }

    printf(".");
    return 0;
}

int test_it_handles_partial_parse ()
{

    int ret = 0;
    char buf[12];
    int bytes_written;
    struct parser_s parser = {0};

    char fst[] = "65";
    char snd[] = "20";

    ret = parser_init(&parser);
    assert(0 == ret);

    ret = parser_parse(&parser, fst, 2);
    assert(1 == ret);

    ret = parser_parse(&parser, snd, 2);
    assert(1 == ret);
    assert(6520 == parser.number);


    printf(".");
    return 0;
}


int main (
    int argc,
    char const* argv[]
)
{

    test_it_parses_0();
    test_it_parses_single_digits();
    test_it_parses_all_valid_numbers();
    test_it_parses_no_invalid_numbers();
    test_it_does_not_parse_negative_numbers();
    test_it_handles_partial_parse();

    printf("\n");
    return 0;
    (void)argc;
    (void)argv;
}
