CC = gcc
CFLAGS = -Wall -lpthread -fsanitize=thread

all: hp test

hp: hp.c
	$(CC) -c -o hp.o hp.c

test: test.c hp.o
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	@echo "Clean..."
	-rm *.o test