#ifndef TIMING_H
#define TIMING_H

#include <stdbool.h>
#include <stdint.h>

#include "worstrun.h"

uint_least64_t cur_time(void);
uint_least64_t delay_time(uint_least64_t, uint_least64_t, uint_least64_t);
bool make_split(struct wr_t *);

#endif
