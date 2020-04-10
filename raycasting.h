#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>


#define sin45 0.707
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

typedef struct {
	double x;
	double y;
} Position;

typedef struct {
	double dx;
	double dy;
} Velocity;
