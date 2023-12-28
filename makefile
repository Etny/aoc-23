CC=gcc
CFLAGS=-Wall -lm

day-%: $(wildcard $@/*.c)
	$(CC) $(CFLAGS) -o $@/out $(wildcard $@/*.c)
