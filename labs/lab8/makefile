CC = g++
CFLAGS = -g -Wall

all: Prog8

Prog8: Support.h Support.cpp Prog8.cpp
	$(CC) $(CFLAGS) -o $@ Prog8.cpp Support.cpp

clean:
	rm -f Prog8
