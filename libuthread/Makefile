# Target library
lib := libuthread.a
targets:= queue
objs := queue.o
CC:= gcc
CFLAGS:= -Wall -Wextra -Werror

all: $(targets)

queue: $(objs)
    $(CC) $(CFLAGS) -c -o $@ $^

all: $(lib)
    ar rcs libuthread.a. $(objs)

