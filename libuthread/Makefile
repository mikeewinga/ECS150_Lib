# Target library
target:= libuthread.a
objs:= queue.o uthread.o context.o preempt.o
CC:= gcc
CFLAGS:= -Wall -Werror
CFLAGS+= -g
PANDOC:= pandoc
deps:= $(patsubst %.o, $.d,$(objs))
-include $(deps)
depflags:= -MMD -M $(@:.o=.d)

ifneq ($(D), 1)
Q = @
endif

$(target): $(objs)
	@echo "AR $(target)"
	$(Q)ar rcs $(target) $(objs)

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) -c $(CFLAGS) $< $(depflags)

clean:
	@echo "CLEANED"
	$(Q) rm -f $(target) $(objs) $(deps) 
