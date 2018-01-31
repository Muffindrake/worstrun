#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <ncurses.h>

#include "config.h"
#include "emit.h"
#include "interface.h"
#include "timing.h"
#include "util.h"

WINDOW *w_top;
WINDOW *w_split;
WINDOW *w_bot;
WINDOW *w_stat;

void
init_interface(void)
{
        initscr();
        raw();
        cbreak();
        noecho();
        curs_set(0);

        check_min_size();
        reset_windows();
}

void
deinit_interface(void)
{
        endwin();
}

int
min_size(void)
{
        return TOP_HEIGHT + BOT_HEIGHT + STATUS_HEIGHT + 1;
}

void
clear_all(void)
{
        wclear(w_top);
        wclear(w_split);
        wclear(w_bot);
        wclear(w_stat);
}

void
reset_windows(void)
{
        int max_y;

        check_min_size();
        max_y = getmaxy(stdscr);
        if (w_top) {
                delwin(w_top);
                delwin(w_split);
                delwin(w_bot);
                delwin(w_stat);
        }
        w_top = newwin(TOP_HEIGHT, 0, 0, 0);
        w_split = newwin(max_y - (TOP_HEIGHT + BOT_HEIGHT + 1), 0, TOP_HEIGHT, 
                        0);
        w_bot = newwin(BOT_HEIGHT, 0, max_y - BOT_HEIGHT - STATUS_HEIGHT, 0);
        w_stat = newwin(STATUS_HEIGHT, 0, max_y - STATUS_HEIGHT, 0);
        keypad(w_stat, TRUE);
}

void
check_min_size(void)
{
        int max_y;

        while (1) {
                max_y = getmaxy(stdscr);
                if (max_y >= min_size())
                        break;
                clear();
                printw("Your terminal must be at least %d rows tall, "
                                "currently %d.\n",
                                min_size(),
                                max_y);
                refresh();
                while (wgetch(w_stat) != KEY_RESIZE);
        }
}

void
split_scroll_up(uint_least64_t *p, uint_least64_t n_split)
{
        if (!n_split)
                *p = 0;
        else if (*p)
                *p -= 1;
        else
                *p = n_split - 1;
}

void
split_scroll_down(uint_least64_t *p, uint_least64_t n_split)
{
        if (!n_split)
                *p = 0;
        else if (*p == n_split - 1)
                *p = 0;
        else
                *p += 1;
}

void
line_clear(WINDOW *w, int y)
{
        wmove(w, y, 0);
        wclrtoeol(w);
}

void
draw_title(WINDOW *w, int y, const char *s)
{
        wmove(w, y, 0);
        if (s) {
                wattron(w, A_BOLD);
                waddstr(w, s);
                wattroff(w, A_BOLD);
        }
        wclrtoeol(w);
}

void
draw_splits(WINDOW *w, struct split *slt, size_t sz, size_t cur)
{
        char tbuf[32];
        size_t i;
        size_t y;
        size_t strl;
        int max_y;
        int max_x;

        i = 0;
        getmaxyx(w, max_y, max_x);
        if (sz > (size_t) max_y)
                i = sz - (size_t) max_y > cur ? cur : sz - (size_t) max_y;

        for (y = 0; y < (size_t) max_y && i < sz; i++, y++) {
                strl = strlen(time_fmt_ms(tbuf, sizeof tbuf, slt[i].time.cur));
                wattron(w, i == cur ? A_BOLD | A_REVERSE : A_BOLD);
                if (slt[i].desc) {
                        wmove(w, y, 0);
                        waddstr(w, slt[i].desc);
                }
                
                wmove(w, y, max_x - (int) strl >= 0 ? max_x - (int) strl : 0);
                waddstr(w, tbuf);
                wattroff(w, i == cur ? A_BOLD | A_REVERSE : A_BOLD);
                if (y != (size_t) max_y - 1 && i != sz - 1)
                        wclrtobot(w);
        }
}

void
draw_time_rightalign(WINDOW *w, int y, const char *s)
{
        size_t strl;
        int max_x;

        max_x = getmaxx(w);
        strl = strlen(s);
        wmove(w, y, max_x - (int) strl >= 0 ? max_x - (int) strl : 0);
        waddstr(w, s);
}

void
draw_string(WINDOW *w, int y, const char *s)
{
        wmove(w, y, 0);
        waddstr(w, s);
}

void
draw_info(WINDOW *w, struct wr_t *wr)
{
        char tbuf[32];

        draw_string(w, BOT_PB_Y, PB_TIME_STRING);
        draw_time_rightalign(w, BOT_PB_Y,
                        time_fmt_ms(tbuf, sizeof tbuf, wr->time.last));
        draw_string(w, BOT_WR_Y, RECORD_TIME_STRING);
        draw_time_rightalign(w, BOT_WR_Y,
                        time_fmt_ms(tbuf, sizeof tbuf, wr->time.record));
}

void
draw_stat(WINDOW *w, const char *s)
{
        wclear(w);
        waddstr(w, s);
        wrefresh(w);
}

void
draw_timer(WINDOW *w, struct wr_t *wr)
{
	char tbuf[32];

	line_clear(w, TOP_TIMER_Y);
	wattron(w, A_BOLD);
	draw_time_rightalign(w, TOP_TIMER_Y, time_fmt(tbuf, sizeof tbuf,
	                wr->time.cur));
	wattroff(w, A_BOLD);
}

void
redraw_all(struct wr_t *wr)
{
        clear_all();
        draw_title(w_top, TOP_TITLE_Y, wr->game);
        draw_string(w_top, TOP_CATEG_Y, wr->categ);
        draw_splits(w_split, wr->gap, wr->gap_sz, wr->cur_split);
        draw_info(w_bot, wr);
}

bool
handle_scrolling(struct wr_t *wr, int ch)
{
        uint_least8_t i;
        int max_y;
        bool rdw;

        rdw = 0;
        switch (ch) {
        case 'k':
        case KEY_UP:
                rdw = 1;
                split_scroll_up(&wr->cur_split, wr->gap_sz);
                break;
        case 'K':
                rdw = 1;
                for (i = 0; i < 5; i++)
                        split_scroll_up(&wr->cur_split, wr->gap_sz);
                break;
        case 'j':
        case KEY_DOWN:
                rdw = 1;
                split_scroll_down(&wr->cur_split, wr->gap_sz);
                break;
        case 'J':
                rdw = 1;
                for (i = 0; i < 5; i++)
                        split_scroll_down(&wr->cur_split, wr->gap_sz);
                break;
        case KEY_HOME:
                if (!wr->gap_sz)
                        break;
                rdw = 1;
                wr->cur_split = 0;
                break;
        case KEY_END:
                if (!wr->gap_sz)
                        break;
                rdw = 1;
                wr->cur_split = wr->gap_sz - 1;
                break;
        }

        max_y = getmaxy(w_split);
        if ((size_t) max_y < wr->gap_sz)
                rdw = 1;
        if (rdw) {
                wclear(w_split);
                draw_splits(w_split, wr->gap, wr->gap_sz, wr->cur_split);
        }
        return rdw;
}

void
reset_split_times(struct wr_t *wr)
{
        size_t i;

        for (i = 0; i < wr->gap_sz; i++)
                wr->gap[i].time.cur = wr->gap[i].time.last;
}

void
refresh_eff(void)
{
        wnoutrefresh(w_top);
        wnoutrefresh(w_split);
        wnoutrefresh(w_bot);
        doupdate();
}

void
loop_main(struct wr_t *wr)
{
        int ch;
        bool rfr;

        draw_stat(w_stat, IDLE_PROMPT);
start:
        ch = wgetch(w_stat);
        switch (ch) {
        case RUN_TOGGLE_K:
                loop_run(wr);
                goto start;
        case MAIN_JSON_EMIT_K:
                if (!wr->gap_sz)
                        goto start;
                if (wr->cur_split != wr->gap_sz - 1)
                        goto start;
                jsontofile(wr, DIRPATH_JSON);
                draw_stat(w_stat, FOUT_PROMPT);
                wrefresh(w_stat);
                goto start;
        case MAIN_QUIT_K:
                return;
        case KEY_RESIZE:
                check_min_size();
                reset_windows();
                redraw_all(wr);
                refresh_eff();
                wtimeout(w_stat, -1);
        }

        rfr = handle_scrolling(wr, ch);
        if (rfr)
                wrefresh(w_split);
        goto start;
}

void
loop_run(struct wr_t *wr)
{
        int ch;
        char tbuf[32];
        const char *s;

        wtimeout(w_stat, TIMEOUT_RUN);
        wr->cur_split = 0;
        reset_split_times(wr);
        redraw_all(wr);
        refresh_eff();
        wr->time.init = cur_time();
        wr->time.cur = 0;
        draw_stat(w_stat, RUN_PROMPT);
start:
        ch = wgetch(w_stat);
        switch (ch) {
        case MAKE_SPLIT_K:
                if (make_split(wr))
                        goto finish;
                wclear(w_split);
                draw_splits(w_split, wr->gap, wr->gap_sz, wr->cur_split);
                wnoutrefresh(w_split);
                wr->time.cur = delay_time(wr->time.init, wr->time.delay,
                                cur_time());
		draw_timer(w_top, wr);
		wnoutrefresh(w_top);
		doupdate();
                goto start;
        case RUN_TOGGLE_K:
                break;
        case RUN_PAUSE_TOGGLE_K:
                loop_pause(wr, TIMEOUT_RUN);
                goto lr_def;
        case KEY_RESIZE:
                check_min_size();
                reset_windows();
                redraw_all(wr);
                wnoutrefresh(w_split);
                wnoutrefresh(w_bot);
                /* fallthrough */
lr_def: default:
                wr->time.cur = delay_time(wr->time.init, wr->time.delay, 
                                cur_time());
		draw_timer(w_top, wr);
                wnoutrefresh(w_top);
                wnoutrefresh(w_stat);
                doupdate();
                wtimeout(w_stat, TIMEOUT_RUN);
                goto start;
        }

        wtimeout(w_stat, -1);
        draw_stat(w_stat, IDLE_PROMPT);
        return;

finish:
        wtimeout(w_stat, -1);
        if (wr->time.cur < wr->time.record)
                s = WR_PROMPT;
        else if (wr->time.cur < wr->time.last)
                s = PB_PROMPT;
        else
                s = FK_PROMPT;
        redraw_all(wr);
        draw_time_rightalign(w_top, TOP_TIMER_Y,
                        time_fmt_ms(tbuf, sizeof tbuf, wr->time.cur));
        draw_stat(w_stat, s);
        wnoutrefresh(w_stat);
        refresh_eff();
        return;
}

void
loop_pause(struct wr_t *wr, int delay)
{
        int ch;
        uint_least64_t pause;

        pause = cur_time();
        draw_stat(w_stat, PAUSE_PROMPT);
        wtimeout(w_stat, -1);
        while (1) {
                ch = wgetch(w_stat);
                if (ch == RUN_PAUSE_TOGGLE_K) {
                        break;
                } else if (ch == KEY_RESIZE) {
                        check_min_size();
                        reset_windows();
                        redraw_all(wr);
                        refresh_eff();
                }
        }
        wr->time.init += cur_time() - pause;
        wtimeout(w_stat, delay);
        draw_stat(w_stat, RUN_PROMPT);
}
