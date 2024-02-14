
CC = g++
CFLAGS = -g -std=c++11
BIN = Prog2

all: $(BIN)

Prog2: Prog2.cpp Support.h
	$(CC) $(CFLAGS) -o Prog2 Prog2.cpp Support.h

clean:
	@rm -f $(BIN)
	@rm -f stdsort_list*txt
	@rm -f qsort_list*txt
	@rm -f qselect_list*txt
