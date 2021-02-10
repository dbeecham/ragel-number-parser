#include <stdio.h>
#include <syslog.h>
#include "parser.h"

%%{

    machine number;
    access parser->;

    action copy_digit {
        parser->number *= 10;
        parser->number += (*p - '0');
    }

    # This parser accepts numbers from 0 to 65205.
    main := (
        # If it's a 0, we're done immediately, nothing else to do.
        '0' $copy_digit
        
        # low numbers - 1-5, 10000-19999, 50000-59999,
        | [1-5] $copy_digit
            ( [0-9]{0,4} $copy_digit )

        # high numbers - 7-9, 70-79, 90-99, 700-799, 900-999, 7000, 9999
        | [7-9] $copy_digit
            ( [0-9]{0,3} $copy_digit )

        # breaking point
        | '6' $copy_digit (
            
            # low numbers: 60 - 64, 60xxx - 64xxx
            [0-4] $copy_digit
                # 60000, 64999
                ( [0-9]{0,3} $copy_digit )

            # high numbers: 66, 69, 66xxx, 69xxx
            | [6-9] $copy_digit
              ( [0-9]{0,2} $copy_digit )


            # breaking point: 65, 65xxxx
            | '5' $copy_digit (

                # low numbers: 650-651
                [0-1] $copy_digit
                    # 6500, 6509, 5419
                    ( [0-9]{0,2} $copy_digit )?

                # high numbers: 653, 653x, 659, 659x
                | [3-9] $copy_digit
                    ( [0-9] $copy_digit )?


                # breaking point: 652, 652x, 652xx
                | '2' $copy_digit (

                    # high numbers: 6521, 6529
                    [1-9] $copy_digit

                    # breaking point: 6520, 6420x
                    | '0' $copy_digit

                        # low numbers: 65200, 65205
                        ( [0-5] $copy_digit )?

                )?
            )?
        )?
    );

    write data;

}%%

int parser_init (
    struct parser_s * parser
) {
    %% write init;
    parser->number = 0;
    return 0;
}

int parser_parse (
    struct parser_s * parser,
    char * buf,
    int buf_len
) {

    syslog(LOG_DEBUG, "%s:%d:%s: hi!", __FILE__, __LINE__, __func__);

    if (buf_len < 0) {
        syslog(LOG_ERR, "%s:%d:%s: buf_len cannot be negative", __FILE__, __LINE__, __func__);
        return -1;
    }

    const char * p = buf;
    const char * pe = buf + buf_len;

    %% write init;
    %% write exec;

    // Did we reach a final state?
    if (%%{ write first_final; }%% <= parser->cs) {
        return 1;
    }

    // Did we reach an error state?
    if (%%{ write error; }%% == parser->cs) {
        return -1;
    }

    return 0;
}
