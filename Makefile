CC=gcc
CFLAGS=-Wall -ISDL2 -D_REENTRANT
TARGET=raycaster

SDL=-lSDL2 -lSDL2_image


all: raycasting.o
	$(CC) -o $(TARGET) $(SDL) raycasting.o

raycasting.o: raycasting.c raycasting.h
	$(CC) -c $(CFLAGS) raycasting.c

clean:
	rm $(TARGET) raycasting.o