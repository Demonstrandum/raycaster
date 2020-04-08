#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

int main(void)
{
    //initiliase graphics system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initliasing SDL: %s\n", SDL_GetError());
        return 1;
    }
    printf("initalisation successful\n");

    //pointer to the window
    SDL_Window* win = SDL_CreateWindow("Testing",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!win) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //create a renderer to set up graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //load image into regular memory
    SDL_Surface* surface = IMG_Load("penis.jpg");
    if (!surface) {
        printf("error creating surface: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load image into GPU (or GPU allocated) Memory
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    //can free the surface once loaded into GPU memory
    SDL_FreeSurface(surface);
    if (!tex) {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //-----------------------------------------------------------------------------------animation

    // struct to hold the position of a spriate
    SDL_Rect dest;

    // get dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 4;
    dest.h /= 4;

    //position in the centre
    float x_pos = (WINDOW_WIDTH - dest.w) / 2 ;
    float y_pos = (WINDOW_HEIGHT - dest.h) / 2;

    float x_vel = 300;
    float y_vel = 300;

    int close_requested = 0;

    while (!close_requested) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close_requested = 1;
            }
        } //collision detection
        if (x_pos <= 0) {
            x_pos = 0;
            x_vel = -x_vel - (rand() % (100 + 1 + 100) - 100);
        } if (y_pos <= 0) {
            y_pos = 0;
            y_vel = -y_vel - (rand() % (100 + 1 + 100) - 100);
        } if (x_pos >= WINDOW_WIDTH - dest.w) {
            x_pos = WINDOW_WIDTH - dest.w;
            x_vel = -x_vel  - (rand() % (100 + 1 + 100) - 100);
        } if (y_pos >= WINDOW_HEIGHT - dest.h) {
            y_pos = WINDOW_HEIGHT - dest.h;
            y_vel = -y_vel - (rand() % (100 + 1 + 100) - 100);
        }
        
        //position updating
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        //set the y pos in the struct, casted to an int (le pixels)
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;

        //draw image to window
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend); //for double buffering (you don't care)

    }


    // //animation loop

    // while (dest.y <= dest.h) {
    //     //clear the window
        // SDL_RenderClear(rend);

        // //set the y pos in the struct, casted to an int (le pixels)
        // dest.y = (int) y_pos;

        // //draw image to window
        // SDL_RenderCopy(rend, tex, NULL, &dest);
        // SDL_RenderPresent(rend); //for double buffering (you don't care)

    //     //update spriate position
    //     y_pos += (float) 1000 / 60;

    //     //wait 1/60th of a second
    //     SDL_Delay(1000/60);
    // }

    // dest.y = (WINDOW_HEIGHT - dest.h) / 2;
    // float x_pos = -WINDOW_WIDTH;
    // dest.h = dest.h/4;
    // dest.w = dest.w/4;

    // while (dest.x <= WINDOW_WIDTH) {
    //     //clear the window
    //     SDL_RenderClear(rend);

    //     //set the y pos in the struct, casted to an int (le pixels)
    //     dest.x = (int) x_pos;

    //     dest.h += 10;
    //     dest.w += 10;

    //     //draw image to window
    //     SDL_RenderCopy(rend, tex, NULL, &dest);
    //     SDL_RenderPresent(rend); //for double buffering (you don't care)

    //     //update spriate position
    //     x_pos += (float) 500 / 60;

    //     //wait 1/60th of a second
    //     SDL_Delay(1000/60);
    // }
    //-----------------------------------------------------------------------------------end of animation


    //cleans up stuff before exit
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}