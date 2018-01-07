#include <errno.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#include "config.h"
#include "interface.h"
#include "parse.h"
#include "util.h"
#include "worstrun.h"

void
initial(int argc, char **argv, struct wr_t *wr)
{
        FILE *f;
        const char *s;
        char buf[1 << 12];

        if (argc == 1 && !strlen(FILEPATH_DEF)) {
                s = getenv("HOME");
                if (!s)
                        s = ".";
                snprintf(buf, sizeof buf, "%s/" FILEN_DEF, s);
                s = buf;
        } else if (argc == 1 && strlen(FILEPATH_DEF)) {
                s = FILEPATH_DEF;
        } else {
                s = argv[1];
        }

        errno = 0;
        f = fopen(s, "r");
        if (!f) {
                HCF("Unable to open file %s: %s", s, strerror(errno));
                exit(EXIT_FAILURE);
        }

        fparse(f, wr);
        fclose(f);
        s = getenv(WR_TERM_TITLE);
        if (s)
                set_term_title(s);
        else
                set_term_title(TERM_TITLE);
}

int
main(int argc, char **argv)
{
        struct wr_t wr = {0};
        size_t i;

        setlocale(LC_ALL, "");
        if (argc > 2 || !argc) {
                fprintf(stderr, "Expected 1 or 0 arguments, got %d\n",
                                argc - 1 >= 0 ? argc - 1 : 0);
                exit(EXIT_FAILURE);
        }

        initial(argc, argv, &wr);
        init_interface();

        redraw_all(&wr);
        refresh_eff();
        loop_main(&wr);

        deinit_interface();
        if (wr.gap) {
                for (i = 0; i < wr.gap_sz; i++)
                        free(wr.gap[i].desc);
                free(wr.gap);
        }
        free(wr.game);
        free(wr.categ);
        return 0;
}
