CFLAGS = -o
STD = -std=c++11
CC = g++
LIBS = -I/usr/local/include -L/usr/local/lib -lnoise

BM: bloodmoon.o
	$(CC) bloodmoon.o $(CFLAGS) bloodmoon.out $(LIBS)

bloodmoon.o:
	$(CC) $(STD) -c bloodmoon.cpp

map: mapexample.o
	$(CC) mapexample.o $(CFLAGS) map.out $(LIBS)

mapexample.o:
	$(CC) $(STD) -c mapexample.cpp

rebuild:
	$(MAKE) clean

clean:
	rm -f core *.o *.out
