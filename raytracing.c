#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_HANDELED
#include "SDL2/SDL.h"

void game_init(void);
void game_quit(void);

static struct {
    SDL_bool running;

    void (*init)(void);
    void (*quit)(void);
}

Game = {
    SDL_FALSE, game_init, game_quit
};

void game_init(void) {
    printf("game_init()\n");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL error -> %s\n", SDL_GetError());
        exit(1);
    }
    Game.running = SDL_TRUE;
}

void game_quit(void) {
    printf("game_quit()\n");
    SDL_Quit();
    Game.running = SDL_FALSE;
}

int main(int argc, char* argv[]) {
    Game.init();

    printf("running = %d\n", Game.running);

    while (Game.running) {
        Game.quit();
    }

    printf("running = %d\n", Game.running);

    return 0;
}
