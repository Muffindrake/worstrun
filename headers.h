#ifndef MAIN_HEADERS
#define MAIN_HEADERS

#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <strings.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

static inline
void
__die(void){abort();}

static inline
int
fileexists(const char s[static 1]){return access(s,F_OK);}

#ifndef _POSIX_OPEN_MAX
#define _POSIX_OPEN_MAX _SC_OPEN_MAX
#endif

#endif
