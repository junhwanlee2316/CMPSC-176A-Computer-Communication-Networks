CC = gcc
CFLAGS = -Wall -Wextra -pedantic

all: PingClient

PingClient: PingClient.o
	$(CC) $(CFLAGS) -o PingClient PingClient.o

PingClient.o: PingClient.c
	$(CC) $(CFLAGS) -c PingClient.c

clean:
	rm -f PingClient PingClient.o
