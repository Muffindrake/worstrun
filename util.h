#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define HCF(fmt, ...) do { \
        fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, \
                        __VA_ARGS__); \
        } while (0)

void set_term_title(const char *);
char *time_fmt(char *, size_t, uint_least64_t);
char *time_fmt_ms(char *, size_t, uint_least64_t);

#endif
