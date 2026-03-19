CC = gcc
CFLAGS = -Wall -Wextra -I Include

SRC = $(wildcard src/*.c)
TEST = test.c

server: $(SRC) $(TEST)
	$(CC) $(CFLAGS) $(SRC) $(TEST) -o server -lpthread

run: server
	./server

clean:
	rm -f server