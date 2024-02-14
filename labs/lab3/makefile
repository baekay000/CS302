
CC = g++
CFLAGS = -Wall
BIN = Prog3

all: $(BIN)

prog3: Support.h Prog3.cpp
	$(CC) $(CFLAGS) -o Prog3 Prog3.cpp

clean:
	rm -f $(BIN)
	@rm -f insertion_*txt
	@rm -f mergesort_*txt
	@rm -f stdsort_*txt
	@rm -f quicksort_*txt
