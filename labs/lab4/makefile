CC = g++
CFLAGS = -Wall

all: Prog4-Crypto1 Prog4-Crypto2 
# Prog4-Crypto3 Prog4-Hoffman

Prog4-Crypto1: Prog4-Crypto1.cpp 
	$(CC) $(CFLAGS) -o $@ Prog4-Crypto1.cpp ppm.cpp

Prog4-Crypto2: Prog4-Crypto2.cpp 
	$(CC) $(CFLAGS) -o $@ Prog4-Crypto2.cpp ppm.cpp

Prog4-Crypto3: Prog4-Crypto3.cpp 
	$(CC) $(CFLAGS) -o $@ Prog4-Crypto3.cpp ppm.cpp

Prog4-Hoffman: Prog4-Hoffman.cpp 
	$(CC) $(CFLAGS) -o $@ Prog4-Hoffman.cpp

clean:
	rm -f Prog4-Crypto? 
	rm -f Prog4-Hoffman
	rm -f *_wmsg.ppm
