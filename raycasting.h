#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define SPEED 300
#define sin45 0.707
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
#define MAP_WIDTH 6
#define MAP_HEIGHT 6

typedef struct {
	double x;
	double y;
} Position;

typedef struct {
	double dx;
	double dy;
} Velocity;

typedef struct {
    double x; double y;
} Direction;

typedef struct {
    double x, y;
} Plane;
