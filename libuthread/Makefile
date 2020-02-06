# Target library
lib := libuthread.a
targets:= queue uthread context preempt
objs := queue.o uthread.o context.o preempt.o
CC:= gcc
CFLAGS:= -Wall -Wextra -Werror

all: $(targets)

queue: queue.o
    $(CC) $(CFLAGS) -c -o $@ $^

uthread: $(objs)
    $(CC) $(CFLAGS) -c -o $@ $^

context: uthread.o context.o preempt.o
    $(CC) $(CFLAGS) -c -o $@ $^

preempt: uthread.o preempt.o
    $(CC) $(CFLAGS) -c -o $@ $^

all: $(lib)
    ar rcs libuthread.a. $(objs)

clean:
    rm -rf $(targets) *.o

