CC=gcc
CFLAGS=-I.
LDFLAGS=-pthread
playfair: playfair.o
	$(CC) -o playfair playfair.o $(LDFLAGS)

crc: crc.o
	$(CC) -o crc crc.o
