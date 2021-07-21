CC:=gcc
CFLAGS:=-Wall
CPPFLAGS:=-I$(CURDIR)
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)
BIN:=code

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(BIN) $(OBJS)
