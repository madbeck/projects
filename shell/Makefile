CFLAGS = -g3 -Wall -Wextra -Wconversion -Wcast-qual -Wcast-align -g
CFLAGS += -Winline -Wfloat-equal -Wnested-externs
CFLAGS += -pedantic -std=gnu99 -Werror
CFLAGS += -D_GNU_SOURCE -std=gnu99

PROMPT = -DPROMPT

CC = gcc
EXECS = 33noprompt 33sh
#do we still want both of these?

.PHONY: clean all

all: $(EXECS)

33sh: shell.c jobs.c jobs.h
	#TODO: compile your program, including the -DPROMPT macro
	$(CC) $(CFLAGS) $(PROMPT) $^ -o $@

33noprompt: shell.c jobs.c jobs.h
	#TODO: compile your program without the prompt macro
	$(CC) $(CFLAGS) $^ -o $@

clean:
	#TODO: clean up any executable files that this Makefile has produced
	rm -f $(EXECS)
