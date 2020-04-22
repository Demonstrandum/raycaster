CC=gcc
CFLAGS=-Wall -Wno-switch -ISDL2 -D_REENTRANT
TARGET=raycaster
OBJ=raycasting.o log.o display.o

LINKS=-lm -lSDL2 -lSDL2_image


all: $(OBJ)
	$(CC) -o $(TARGET) $(LINKS) $(OBJ)

raycasting.o: raycasting.c raycasting.h
	$(CC) -c $(CFLAGS) raycasting.c

log.o: log.c log.h
	$(CC) -c $(CFLAGS) log.c

display.o: display.c display.h
	$(CC) -c $(CFLAGS) display.c

clean:
	rm -f $(TARGET) $(OBJ)
