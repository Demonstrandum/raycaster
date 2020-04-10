#include "raycasting.h"

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


    // struct to hold the position of a spriate
    SDL_Rect dest;

    // get dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 4;
    dest.h /= 4;

    //position in the centre
    Position pos = {
    	.x = (WINDOW_WIDTH  - dest.w) / 2,
	.y = (WINDOW_HEIGHT - dest.h) / 2
    };

    Velocity vel = { .dx = 0, .dy = 0};


    //keyboard inputs
    // int up = 0;
    // int down = 0;
    // int left = 0;
    // int right = 0;

    int vert = 0;
    int hoz = 0;

    // to allow the closing of the program
    bool close_requested = false;
    //main game loop
    while (!close_requested) {
        //creates the new event
        SDL_Event event;
        //while there is some event lined up on the queue, if no events it skips the while loop 
        while (SDL_PollEvent(&event)) {
            //check for the type of the event
            switch (event.type) {
                case SDL_QUIT:
                    close_requested = true;
                    break;
                case SDL_KEYDOWN:
                    //event.key gets the "key" structure from the union, since the union (struct) can only
                    //have one variable at time, we have to choose one, just read the SDL_EVENT docs smh
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            //printf("W DOWN\n");
                            vert = 1;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            //printf("S DOWN\n");
                            vert = -1;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            //printf("D DOWN\n");
                            hoz = 1;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            // printf("A DOWN\n");
                            hoz = -1;
                            break;
			default:
			    break;
                    } break; // Remember to break out again for the entire case
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            //printf("W UP\n");
                            vert = 0;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            //printf("S UP\n");
                            vert = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            //printf("D UP\n");
                            hoz = 0;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            //printf("A UP\n");
                            hoz = 0;
                            break;
			default:
			    break;
                    } break;
            }
        }
        //main animation loop runs; if no event is detected 
        
        //determining the velocity
        vel.dx = vel.dy = 0;

        //if (up && right && !down && !left) x_vel = 212, y_vel = -212;

        if (vert != 0 && hoz != 0) {
            vel.dy = 300 * sin45 * -vert;
            vel.dx = 300 * sin45 * hoz;
        }
        if (!vert && hoz != 0) vel.dx = 300 * hoz;
        if (!hoz && vert != 0) vel.dy = 300 * -vert;

        // if (up && !down) y_vel = -300; //top of window is 0
        // if (!up && down) y_vel = 300;
        // if (right && !left) x_vel = 300;
        // if (!right && left) x_vel = -300;

        //position updating
        pos.x += vel.dx / 60;
        pos.y += vel.dy / 60;

        //collision detection
        if (pos.x <= 0)
            pos.x = 0;
        else if (pos.x >= WINDOW_WIDTH - dest.w)
            pos.x = WINDOW_WIDTH - dest.w;

	if (pos.y <= 0)
	    pos.y = 0;
	else if (pos.y >= WINDOW_HEIGHT - dest.h)
            pos.y = WINDOW_HEIGHT - dest.h;

        //set the y pos in the struct that holds the sprite, casted to an int (le pixels)
        dest.y = (int) pos.y;
        dest.x = (int) pos.x;

        //clear and draw image to window
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend); //for double buffering (you don't care)

        //SDL_Delay(1000/60);
    } //end of game loop


    //cleans up stuff before exit
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
