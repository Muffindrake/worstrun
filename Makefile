.PHONY: default
default: all

V := wah1
V_SUFFIX ?=
VERSION := \"$(V)$(V_SUFFIX)\"
OPTLEVEL ?= -Os
WARLEVEL ?= -Wall -Wextra -Wpedantic -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-unused-command-line-argument

COSMOCFLAGS := -g $(OPTLEVEL) -std=c2x $(WARLEVEL) -static -no-pie -fno-pie -mno-red-zone -nostdlib -nostdinc -include cosmopolitan.h -MMD -DVERSION=$(VERSION)
COSMOLDFLAGS := -fuse-ld=bfd -Wl,-T,ape.lds crt.o ape.o cosmopolitan.a

CFLAGS := $(OPTLEVEL) -std=c2x $(WARLEVEL) -MMD -DVERSION=$(VERSION) -include headers.h

COSMOSRCS := $(wildcard *.c)
COSMODEPS := $(COSMOSRCS:.c=.d) wr.com.d
COSMOOBJS := $(COSMOSRCS:.c=.o)

SRCS := $(wildcard *.c)
DEPS := $(SRCS:.c=.d)
OBJS := $(SRCS:.c=.o)

cosmo_files: ape.lds ape.o crt.o cosmopolitan.h cosmopolitan.a

.PHONY: cosmo
cosmo: $(COSMOSRCS) cosmo_files
	$(CC) $(COSMOCFLAGS) -o wr.com.dbg $(COSMOSRCS) $(COSMOLDFLAGS)
	objcopy -SO binary wr.com.dbg wr.com

.PHONY: all
all: $(SRCS)
	$(CC) $(CFLAGS) -o wr $(SRCS) $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) wr wr.com wr.com.dbg $(OBJS) $(DEPS) $(COSMOOBJS) $(COSMODEPS)
