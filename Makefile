CFLAGS = -o
CC = g++

MG: main.o 
	$(CC) main.o $(CFLAGS) MG.out -I ./Noise/ -L -/Noise -lnoise

main.o:
	$(CC) -c main.cpp

clean:
	rm -f core *.o
