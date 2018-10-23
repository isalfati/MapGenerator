CFLAGS = -o
STD = -std=c++11
CC = g++

MG: main.o noiseutils.o
	$(CC) main.o $(CFLAGS) MG.out -I ./Noise/ -L -/Noise -lnoise

main.o:
	$(CC) $(STD) -c main.cpp

noiseutils.o:
	$(CC) $(STD) -c noiseutils.cpp

clean:
	rm -f core *.o
