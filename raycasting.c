#include "raycasting.h"

int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1},
    {1,0,0,0,0,1},
    {1,0,0,2,0,1},
    {1,0,0,2,0,1},
    {1,0,0,2,0,1},
    {1,1,1,1,1,1}
};

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

    //load image into regular memory, pointer obvs
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

    //creating le rectangle (maybe)
    //SDL_Rect dest_box = {10, 10, 10, 10};


    // struct to hold the position of a spriate
    SDL_Rect dest;

    // get dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 4;
    dest.h /= 4;



    //player information
    Position pos = {.x = 2, .y = 2};
    Velocity vel = { .dx = 0, .dy = 0};
    //plane and direction MUST be parallel
    Plane pln = {0.70, 0};
    Direction dir = {0, 1};

    double pln_x = 0.70, pln_y = 0;
    double dir_x = 0, dir_y = 1;


    struct Player {
        Position pos;
        Velocity vel;
        Direction dir;
        Plane pln;
    };

    struct Player player = {
        pos, vel, dir, pln
    };

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
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            vert = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            hoz = 0;
                            break;
			default:
			    break;
                    } break;
            }
        }
        //main animation loop runs; if no event is detected 
        
        //ray position and direction
        for (int i = 0; i <= WINDOW_WIDTH; i++) {
            //length of rays, lambda is the scalar that essentially moves the ray across the camera plane
            double lambda = (2 * i) / WINDOW_WIDTH - 1;
            double ray_x = dir_x + pln_x * lambda;
            double ray_y = dir_y + pln_y * lambda;

            int map_x = (int) floor(player.pos.x);
            int map_y = (int) floor(player.pos.x);

            //distance from the player to the first x or y side
            double init_x, init_y;

            //length from one x to the next
            double delta_x = sqrt(1 + (ray_y * ray_y) / (ray_x * ray_x));
            double delta_y = sqrt(1 + (ray_x * ray_x) / (ray_y * ray_y));

            //step in either x or y, positive or negative, depending on the direction of the ray (+-)
            int step_x, step_y;

            bool hit = false;
            bool side; //x-side 0, y-side 1

            //calculate the inital distance to the first side
            double mu_x = player.pos.x - map_x;
            double mu_y = player.pos.y - map_y;
            if (ray_x < 0) {
                step_x = -1;
                init_x = mu_x * delta_x;
            } else {
                step_x = 1;
                init_x = (1 - mu_x) * delta_x;
            } if (ray_y < 0) {
                step_y = -1;
                init_y = mu_y * delta_y;
            } else {
                step_y = 1;
                init_y = (1 - mu_y) * delta_y;
            }

            //le DDA
            while (!hit) {
                if (init_x < init_y) {
                    init_x += delta_x;
                    map_x += step_x;
                    side = false;
                } else {
                    init_y += delta_y;
                    map_y += step_y;
                    side = true;
                } if (map[map_x][map_y] > 0) hit = true;
            }


        }



        //---------------------------------------------------------------------------------------
        // //determining the velocity
        // vel.dx = vel.dy = 0;

        // if (vert != 0 && hoz != 0) {
        //     vel.dy = VELOCITY * sin45 * -vert;
        //     vel.dx = VELOCITY * sin45 * hoz;
        // }
        // if (!vert && hoz != 0) vel.dx = VELOCITY * hoz;
        // if (!hoz && vert != 0) vel.dy = VELOCITY * -vert;

        // //position updating
        // pos.x += vel.dx / 60;
        // pos.y += vel.dy / 60;

        // //collision detection
        // if (pos.x <= 0) pos.x = 0;
        // if (pos.x >= WINDOW_WIDTH - dest.w) pos.x = WINDOW_WIDTH - dest.w;

	    // if (pos.y <= 0) pos.y = 0;
	    // if (pos.y >= WINDOW_HEIGHT - dest.h) pos.y = WINDOW_HEIGHT - dest.h;

        // //set the y pos in the struct that holds the sprite, casted to an int (le pixels)
        // dest.y = (int) pos.y;
        // dest.x = (int) pos.x;
        //-----------------------------------------------------------------------------------

        //clear and draw image to window
        SDL_RenderClear(rend);
        //draw order matters!
        //SDL_RenderCopy(rend, tex, NULL, &dest);
        //SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
        //SDL_RenderFillRect(rend, &dest_box);
        //SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);


        SDL_RenderPresent(rend); //for double buffering (you don't care)

        //SDL_Delay(1000/60);
    } //end of game loop


    //cleans up stuff before exit
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
