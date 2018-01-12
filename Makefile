CFLAGS := $(CFLAGS) `pkg-config ncursesw jansson --cflags` \
	-std=c11 -Wall -Wextra -Wpedantic -pipe
LDFLAGS ?= -Wl,-O1 -Wl,--as-needed -Wl,-flto
LIBS := `pkg-config ncursesw jansson --libs`

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

OPTLEVEL ?= -O3 -march=native -flto
prefix ?= /usr/local

%.o: %.c
	$(CC) $(CFLAGS) $(OPTLEVEL) -c $<

wr: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OPTLEVEL) $(OBJS) $(LDFLAGS) $(LIBS)

.PHONY: all
all: wr

.PHONY: clean
clean:
	$(RM) wr $(OBJS)

.PHONY: install
install: all
	install -m 0755 wr $(prefix)/bin
