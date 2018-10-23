CFLAGS = -o
STD = -std=c++11
CC = g++

MG: main.o
#	$(CC) main.o $(CFLAGS) MG.out -I ./Noise/ -L -/Noise -lnoise
	$(CC) main.o $(CFLAGS) MG.out -I/usr/local/include -L/usr/local/lib -lnoise

main.o:
	$(CC) $(STD) -c main.cpp

clean:
	rm -f core *.o *.out
