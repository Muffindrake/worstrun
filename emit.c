#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <jansson.h>

#include "config.h"
#include "emit.h"
#include "interface.h"
#include "keynames.h"
#include "util.h"
#include "worstrun.h"

void
json_setentry(json_t *root, const char *k, json_t *val)
{
        int err;

        if (!val) {
                deinit_interface();
                HCF("Unable to set entry \"%s\" in JSON object: "
                                "pointer to value is null", k);
                exit(EXIT_FAILURE);
        }
        err = json_object_set_new_nocheck(root, k, val);
        if (err) {
                deinit_interface();
                HCF("Unable to set entry \"%s\" in JSON object", k);
                exit(EXIT_FAILURE);
        }
}

void
json_array_setentry(json_t *arr, const char *k, json_t *val)
{
        int err;

        if (!val) {
                deinit_interface();
                HCF("Unable to set entry \"%s\" in splits JSON array: "
                                "pointer to value is null", k ? k : "");
                exit(EXIT_FAILURE);
        }
        err = json_array_append_new(arr, val);
        if (err) {
                deinit_interface();
                HCF("Unable to set entry \"%s\" in splits JSON array",
                                k ? k : "");
                exit(EXIT_FAILURE);
        }
}

void
femit(FILE *f, struct wr_t *wr)
{
        json_t *root;
        json_t *arr;
        size_t i;
        int err;

        root = json_object();
        json_setentry(root, GAME_KEY, json_string_nocheck(wr->game));
        json_setentry(root, CATEG_KEY, json_string_nocheck(wr->categ));
        json_setentry(root, RECORD_KEY, json_integer(wr->time.record));
        json_setentry(root, BEST_KEY, json_integer(wr->time.last));
        json_setentry(root, DELAY_KEY, json_integer(wr->time.delay));
        arr = json_array();
        for (i = 0; i < wr->gap_sz; i++)
                json_array_setentry(arr, wr->gap[i].desc,
                                json_pack("{sssi}",
                                SPLIT_TITLE_KEY,
                                wr->gap[i].desc ? wr->gap[i].desc : "",
                                SPLIT_LAST_KEY,
                                wr->gap[i].time.cur));
        json_setentry(root, SPLIT_KEY, arr);
        err = json_dumpf(root, f, JSON_INDENT(8));
        if (err) {
                deinit_interface();
                HCF("Unable to dump JSON to file%s", "");
                exit(EXIT_FAILURE);
        }
        json_decref(root);
}

void
jsontofile(struct wr_t *wr, const char *path)
{
        FILE *f;
        char buf[1 << 12];
        char *env;

        env = getenv(WR_SPLIT_DIR);
        if (!env && !strlen(path)) {
                path = getenv("HOME");
                if (!path)
                        path = ".";
        } else if (!strlen(path)) {
                path = env;
        }
        snprintf(buf, sizeof buf, "%s/"SPLIT_PREFIX"%llu.json", path,
                        (unsigned long long) time(0));
        errno = 0;
        f = fopen(buf, "w");
        if (!f) {
                deinit_interface();
                HCF("Unable to open file %s: %s", buf, strerror(errno));
                exit(EXIT_FAILURE);
        }
        femit(f, wr);
        fputs("\n", f);
        fclose(f);
}
