#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "timing.h"
#include "worstrun.h"

uint_least64_t
cur_time(void)
{
        struct timespec ts;

        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint_least64_t) ts.tv_sec * 1000
                + (uint_least64_t) ts.tv_nsec / 1000000;
}

uint_least64_t
delay_time(uint_least64_t i, uint_least64_t d, uint_least64_t c)
{
        if (!d)
                return c - i;
        if (c - i < d)
                return 0;
        else
                return c - i - d;
}

bool
make_split(struct wr_t *wr)
{
        wr->time.cur = delay_time(wr->time.init, wr->time.delay, cur_time());
        if (!wr->gap_sz)
                return 1;
        wr->gap[wr->cur_split].time.cur = wr->time.cur;
        if (wr->gap_sz - 1 == wr->cur_split)
                return 1;
        wr->cur_split++;
        return 0;
}
