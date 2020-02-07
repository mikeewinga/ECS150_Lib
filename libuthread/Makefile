# Target library
target:= libuthread.a
objs:= queue.o context.o preempt.o uthread.o
CC:= gcc
CFLAGS:= -Wall -Werror -Wextra
CFLAGS+= -g
PANDOC:= pandoc
deps:= $(patsubst %.o,%.d,$(objs))
-include $(deps)
depflags:= -MMD $(@:.o=.d)

ifneq ($(V),1)
Q = @
endif

$(target): $(objs)
	@echo "AR $(target)"
	$(Q)ar rcs $(target) $(objs)

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(depflags)

clean:
	@echo "CLEANED"
	$(Q) rm -f $(target) $(objs) $(deps) 
