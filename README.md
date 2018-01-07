# worstrun

![preview](https://i.imgur.com/rTdwtOy.png)

This is a minimal split tracker for terminals supported by ncurses.

#### installation

###### dependencies

- C11 compiler    (gcc 4.7+, clang 3.6+)
- POSIX bubble bath (`clock_gettime`)
- ncurses library (runtime and headers)
- jansson library (runtime and headers)
- GNU make

The program will `setlocale(LC_ALL, "")` and use `ncursesw` in order to properly
display UTF-8 text in the terminal, if that is necessary and possible. This
requires a unicode-capable terminal, sufficient font support and a UTF-8 locale.

###### ...

    $ git clone https://github.com/muffindrake/worstrun
    $ cd worstrun
    $ make
    $ ./wr split.json

#### usage

Upon startup, either a default (`"$HOME/.splits.json"`) or specified json file
containing information such as game title, category and split data is read and
the content is displayed.

In IDLE, you're able to:
- `Q`uit the program
- scroll the split contents (key `up`/`down`, `j`/`k`, `home`/`end`)
- commence a run using `F4`
- write a JSON file to the home directory containing current split times (`F12`)

In a RUN:
- record a split using `F8`, ending the run when the last split is recorded
- toggle pause using `F1`
- return to IDLE, ending the run using `F4`

You are able to force-quit at any point by hitting `^C` (Control + C).

#### split data

You should use the offered split file as a hard guideline for writing your
own and you should also use a text editor that points out any invalid json you
may have typed - the program will refuse to read invalid json, as implemented by
the jansson library.

The times inside the file are in integer milliseconds from start of run.

#### configuration

`config.h` contains macro definitions for keybindings (refer to `man 3
curs_getch`), as well as some strings and window heights/coordinates that you
may edit within reason.

The program never writes to any files for any reason, except as noted, as such 
you have to maintain splits by your method of choice.

If you require global hotkeys under X11, it may be a good idea to use `xdotool` 
in conjunction with some hotkey manager.
The program sets the terminal title to `worstrun` or the string contained in the
environment variable `WR_TERM_TITLE`.

#### license

See the LICENSE file.
