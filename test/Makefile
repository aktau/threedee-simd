# example with another compiler: make clean && make CC="/usr/local/bin/clang" && ./test

INC:=-I../deps/
INC+=-I../include/

DBG:=-g3 -ggdb

OPT:=-O3 -ffast-math -march=native

CC?=gcc
STD=-std=c11 -MMD -W -Wall -Wextra -pedantic
WARN:=-Wcast-align -Wcast-qual \
	-Wpointer-arith -Waggregate-return -Wunreachable-code \
	-Wfloat-equal -Wformat=2 -Wredundant-decls \
	-Wundef -Wdisabled-optimization -Wshadow \
	-Wmissing-braces -Wstrict-aliasing=2 -Wstrict-overflow=5 \
	-Wconversion -Wno-unused-parameter \
	-Wno-missing-field-initializers -Wno-missing-braces \
	-Wno-unused-function
ifeq ($(CC),gcc)
	OPT+=-mfpmath=sse
endif
ifneq (,$(findstring clang,$(CC)))
	OPT+=-fslp-vectorize-aggressive
endif
CFLAGS+=$(STD) $(WARN) $(INC) $(DBG) $(OPT)
LDFLAGS=
LDLIBS=-lm
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

all: test

test: $(OBJS)

bench.o: bench.c
	$(CC) $(STD) $(WARN) $(INC) $(DBG) -O0 -c -o $@ $^

# -masm=att / -masm=intel
# objdump -d --disassembler-options=att code.c
# objdump -d --disassembler-options=intel code.c
# $(CC) $(CFLAGS) -S -fverbose-asm test.c
test.s: test
	otool -tV $^ > test.s

$(EXECUTABLE): $(OBJS)

-include $(DEPS)

clean:
	rm -f test
	rm -f $(OBJS) $(DEPS)

.PHONY: all clean
