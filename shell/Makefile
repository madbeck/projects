CFLAGS = -g3 -Wall -Wextra -Wconversion -Wcast-qual -Wcast-align -g
CFLAGS += -Winline -Wfloat-equal -Wnested-externs
CFLAGS += -pedantic -std=gnu99 -Werror
CFLAGS += -D_GNU_SOURCE -std=gnu99

PROMPT = -DPROMPT

CC = gcc
EXECS = noprompt prompt

.PHONY: clean all

all: $(EXECS)

prompt: shell.c jobs.c jobs.h
	$(CC) $(CFLAGS) $(PROMPT) $^ -o $@

noprompt: shell.c jobs.c jobs.h
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(EXECS)
