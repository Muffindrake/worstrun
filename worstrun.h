#ifndef WORSTRUN_H
#define WORSTRUN_H

#include <stddef.h>
#include <stdint.h>

struct split {
        char *desc;

        struct {
                uint_least64_t last;
                uint_least64_t cur;
        } time;
};

struct wr_t {
        char *game;
        char *categ;
        struct split *gap;
        size_t gap_sz;
        size_t cur_split;

        struct {
                uint_least64_t init;
                uint_least64_t delay;
                uint_least64_t last;
                uint_least64_t record;
                uint_least64_t cur;
        } time;
};

#endif
