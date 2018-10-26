CFLAGS = -o
STD = -std=c++11
CC = g++

MG: main.o
	$(CC) main.o $(CFLAGS) MG.out -I/usr/local/include -L/usr/local/lib -lnoise

map: mapexample.o
	$(CC) mapexample.o $(CFLAGS) map.out -I/usr/local/include -L/usr/local/lib -lnoise

mapexample.o:
	$(CC) $(STD) -c mapexample.cpp

main.o:
	$(CC) $(STD) -c main.cpp

rebuild:
	$(MAKE) clean
	$(MAKE) MG

clean:
	rm -f core *.o *.out
