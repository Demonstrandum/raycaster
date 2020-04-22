#pragma once
#include "raycasting.h"

typedef struct {
	float64_t x, y;
} xy_t;

typedef xy_t Position;

typedef struct {
	float64_t dx, dy;
} Velocity;

typedef xy_t Direction;

typedef xy_t Plane;

typedef struct {
    Position position;
    Velocity velocity;
    Direction direction;
    Plane plane;
} Player;
