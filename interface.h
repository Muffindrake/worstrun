#ifndef INTERFACE_H
#define INTERFACE_H

#include <stddef.h>
#include <stdint.h>

#include "worstrun.h"

extern WINDOW *w_top;
extern WINDOW *w_split;
extern WINDOW *w_bot;
extern WINDOW *w_stat;

void init_interface(void);
void deinit_interface(void);
void line_clear(WINDOW *, int);
int min_size(void);
void clear_all(void);
void check_min_size(void);
void reset_windows(void);
void reset_split_times(struct wr_t *);
void refresh_eff(void);
void redraw_all(struct wr_t *);
bool handle_scrolling(struct wr_t *, int);
void split_scroll_up(uint_least64_t *, uint_least64_t);
void split_scroll_down(uint_least64_t *, uint_least64_t);
void draw_game(WINDOW *, const char *);
void draw_title(WINDOW *, int, const char *);
void draw_splits(WINDOW *, struct split *, size_t, size_t);
void draw_timer(WINDOW *, struct wr_t *);
void draw_last_delta(WINDOW *, int, uint_least64_t, uint_least64_t);
void draw_time_rightalign(WINDOW *, int, const char *);
void draw_info(WINDOW *, struct wr_t *);
void draw_stat(WINDOW *, const char *);
void draw_string(WINDOW *, int, const char *);
void loop_run(struct wr_t *);
void loop_main(struct wr_t *);
void loop_pause(struct wr_t *, int);

#endif
