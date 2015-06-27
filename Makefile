CC = gcc
CFLAGS = -ggdb
OBJECTS = server.o client.o

all: server client

server: server.o
	$(CC) $(CFLAGS) server.o -o server

server.o: server.c
	$(CC) -c $(CFLAGS) server.c

client: client.o
	$(CC) $(CFLAGS) client.o -o client

client.o: client.c
	$(CC) -c $(CFLAGS) client.c

.PHONY: clean

clean:
	rm -rf core.* $(OBJECTS) server client

.PHONY: strip

strip:
	strip -g server client
