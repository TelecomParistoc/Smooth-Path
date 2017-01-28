CC = gcc
CFLAGS = -c -std=gnu99 -O2 -Werror -Wall -Wextra
LDLIBS = -lm -lgraph

SRCS = main.c
OBJS = $(subst .c,.o,$(SRCS))
EXE = main

.PHONY : all run clean

all: run clean

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJS)

clean:
	rm -f *.o $(EXE)
