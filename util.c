#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "util.h"

void
set_term_title(const char *t)
{
        fputs("\033]0;", stdout);
        fputs(t, stdout);
        fputs("\007", stdout);
}

char *
time_fmt(char *buf, size_t sz, uint_least64_t time)
{
        time /= 1000;
        uint_least8_t sec       = time          % 60;
        uint_least8_t min       = time / 60     % 60;
        uint_least8_t h         = time / 3600   % 24;
        uint_least16_t d        = time / 86400;
        if (d)
                snprintf(buf, sz, "%"PRIuLEAST16":%02"PRIuLEAST8":%02"PRIuLEAST8
                                ":%02"PRIuLEAST8,
                                d, h, min, sec);
        else if (h)
                snprintf(buf, sz,
                                "%"PRIuLEAST8":%02"PRIuLEAST8":%02"PRIuLEAST8,
                                h, min, sec);
        else if (min)
                snprintf(buf, sz, "%"PRIuLEAST8":%02"PRIuLEAST8, min, sec);
        else
                snprintf(buf, sz, "%"PRIuLEAST8, sec);
        return buf;
}

char *
time_fmt_ms(char *buf, size_t sz, uint_least64_t time)
{
        uint_least16_t ms       = time % 1000;
        time /= 1000;
        uint_least8_t sec       = time          % 60;
        uint_least8_t min       = time / 60     % 60;
        uint_least8_t h         = time / 3600   % 24;
        uint_least16_t d        = time / 86400;
        if (d)
                snprintf(buf, sz, "%"PRIuLEAST16":%02"PRIuLEAST8":%02"PRIuLEAST8
                                ":%02"PRIuLEAST8".%03"PRIuLEAST16,
                                d, h, min, sec, ms);
        else if (h)
                snprintf(buf, sz,
                                "%"PRIuLEAST8":%02"PRIuLEAST8":%02"PRIuLEAST8
                                ".%03"PRIuLEAST16,
                                h, min, sec, ms);
        else if (min)
                snprintf(buf, sz, "%"PRIuLEAST8":%02"PRIuLEAST8
                                ".%03"PRIuLEAST16,
                                min, sec, ms);
        else
                snprintf(buf, sz, "%"PRIuLEAST8".%03"PRIuLEAST16, sec, ms);
        return buf;
}

