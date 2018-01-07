#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include "keynames.h"
#include "parse.h"
#include "util.h"
#include "worstrun.h"

#define JSON_NO_STR(str) HCF("Error: \"%s\" is not a string", str)
#define JSON_NO_INT(str) HCF("Error: \"%s\" is not an integer", str)
#define JSON_NO_ARR(str) HCF("Error: \"%s\" is not an array", str)

static inline
char *
xstrdup(const char *s)
{
        size_t len;
        char *ret;

        len = strlen(s);
        ret = malloc(len + 1);
        if (!ret)
                return 0;
        return strcpy(ret, s);
}

char *
str_json(json_t *root, const char *key)
{
        root = json_object_get(root, key);
        if (!root || json_is_null(root)) {
                return 0;
        } else if (!json_is_string(root)) {
                JSON_NO_STR(key);
                exit(EXIT_FAILURE);
        } else {
                return xstrdup(json_string_value(root));
        }
}

uint_least64_t
int_json(json_t *root, const char *key)
{
        root = json_object_get(root, key);
        if (!root || json_is_null(root)) {
                return 0;
        } else if (!json_is_integer(root)) {
                JSON_NO_INT(key);
                exit(EXIT_FAILURE);
        } else {
                return json_integer_value(root);
        }
}

void
fparse(FILE *f, struct wr_t *wr)
{
        json_t *root;
        json_t *arr;
        json_t *split;
        json_error_t err;
        size_t i;

        root = json_loadf(f, 0, &err);
        if (!root) {
                HCF("Unable to parse JSON: line %d: %s", err.line, err.text);
                exit(EXIT_FAILURE);
        }

        wr->game = str_json(root, GAME_KEY);
        wr->categ = str_json(root, CATEG_KEY);
        wr->time.record = int_json(root, RECORD_KEY);
        wr->time.last = int_json(root, BEST_KEY);
        wr->time.delay = int_json(root, DELAY_KEY);

        arr = json_object_get(root, SPLIT_KEY);
        if (!arr) {
                wr->gap = 0;
                wr->gap_sz = 0;
                goto cleanup;
        } else if (!json_is_array(arr)) {
                JSON_NO_ARR(SPLIT_KEY);
                exit(EXIT_FAILURE);
        }

        wr->gap_sz = json_array_size(arr);
        wr->gap = calloc(wr->gap_sz, sizeof *wr->gap);
        for (i = 0; i < wr->gap_sz; i++) {
                split = json_array_get(arr, i);
                wr->gap[i].desc = str_json(split, SPLIT_TITLE_KEY);
                wr->gap[i].time.last = int_json(split, SPLIT_LAST_KEY);
                wr->gap[i].time.cur = wr->gap[i].time.last;
        }

cleanup:
        json_decref(root);
        return;
}
