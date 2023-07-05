CFLAGS = $(WFLAGS) $(OPTIM)

WFLAGS = -Wall -Wextra -pedantic -std=c99
IFLAGS = -Isrc/

BINARIES = bin/test

.PHONY: default
default: debug

.PHONY: debug
debug: DEBUG = -fsanitize=address,undefined
debug: OPTIM = -g
debug: dirs $(BINARIES)

# tests

bin/test: tests/test.c src/maggot/maggot.h
	$(CC) -o $@ $< $(CFLAGS) $(IFLAGS) $(DEBUG) $(DEFINES)

# dirs

.PHONY: dirs
dirs: bin/

%/:
	mkdir -p $@

# clean

.PHONY: clean
clean:
	$(RM) -r bin/*
