#ifndef CONFIG_H
#define CONFIG_H

#include <ncurses.h>

/* keybindings, either use character literals ('k') or KEY_ macros found in 
 * man 3 curs_getch */
/* do not attempt to use any non-US-ASCII characters here */
/* do not use any key twice or more */
#define MAKE_SPLIT_K            KEY_F(8)
#define RUN_TOGGLE_K            KEY_F(4)
#define RUN_PAUSE_TOGGLE_K      KEY_F(1)
#define MAIN_JSON_EMIT_K        KEY_F(12)
#define MAIN_QUIT_K             'Q'

/* default filename for splits file in home directory */
#define FILEN_DEF               ".splits.json"
/* path to a file that is used instead of the above, set to anything other than
 * an empty string to use */
#define FILEPATH_DEF            ""
/* path to a directory that is used to output split information to a file,
 * set to anything other than an empty string to use, otherwise HOME is used */
/* the output filename is splits-<unix timestamp>.json */
#define DIRPATH_JSON            ""
/* prefix in output filename */
#define SPLIT_PREFIX            "splits-"

/* minimal height used for top/bottom/status window; splits are at least 1 
 * high */
/* layout is stacked windows in the order: top, splits, bot, status */
#define TOP_HEIGHT              3
#define BOT_HEIGHT              2
#define STATUS_HEIGHT           1

/* coordinates of the upper-leftmost cell in a window are (0, 0) in ncurses */

/* layout of the top window */
/* game title */
#define TOP_TITLE_Y             0
/* category title */
#define TOP_CATEG_Y             1
/* run timer */
#define TOP_TIMER_Y             2

/* layout of the bottom window */
/* personal best */
#define BOT_PB_Y                0
/* record time */
#define BOT_WR_Y                1

/* time in milliseconds that ncurses should wait for input before updating
 * the timer while a run is in progress */
/* lower values mean more use of CPU time */
#define TIMEOUT_RUN             1000

#define PB_TIME_STRING          "personal best:"
#define RECORD_TIME_STRING      "       record:"

#define IDLE_PROMPT             "IDLE"
#define RUN_PROMPT              "RUN IN PROGRESS"
#define PAUSE_PROMPT            "PAUSE"
#define WR_PROMPT               "NEW RECORD"
#define PB_PROMPT               "NEW PERSONAL BEST"
#define FK_PROMPT               "BETTER LUCK NEXT TIME"
#define FOUT_PROMPT             "FILE WRITTEN"

/* names of environment variables used to adjust some behaviour */
/* terminal title */
#define WR_TERM_TITLE           "WR_TERM_TITLE"
/* output path for split json */
#define WR_SPLIT_DIR            "WR_SPLIT_DIR"

/* terminal title to set if WR_TERM_TITLE is not set */
#define TERM_TITLE              "worstrun"

#endif
