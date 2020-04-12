CC=gcc
CFLAGS=-Wall -ISDL2 -D_REENTRANT
TARGET=raycaster

LINKS=-lm -lSDL2 -lSDL2_image


all: raycasting.o
	$(CC) -o $(TARGET) $(LINKS) raycasting.o

raycasting.o: raycasting.c raycasting.h
	$(CC) -c $(CFLAGS) raycasting.c

clean:
	rm $(TARGET) raycasting.o
