CFLAGS = -o
CC = g++

MG: main.o mapGenerator.o
	$(CC) main.o mapGenerator.o  $(CFLAGS) MG

main.o:
	$(CC) -c main.cpp

mapGenerator.o:
	$(CC) -c mapGenerator.cpp

clean:
	rm -f core *.o
