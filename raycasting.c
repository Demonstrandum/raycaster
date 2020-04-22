#include "raycasting.h"

// Height map
uint8_t MAP[MAP_WIDTH][MAP_HEIGHT] = {
    { 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2 }
};

SDL_Window   *win     = NULL;
SDL_Renderer *rend    = NULL;
SDL_Surface  *surface = NULL;
SDL_Texture  *tex     = NULL;

int main(int argc, char **argv)
{
    // Initiliase graphics system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        log_fatal("Failed to initliasing SDL: %s", SDL_GetError());
        goto destruction;
    }

    log_info("Initalisation successful!");

    // Initialise pointer to the window
    win = SDL_CreateWindow(
        "Testing",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!win) goto destruction;

    // Create a renderer to set up graphics hardware
    uint32_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend) goto destruction;

    // Load image into regular memory, pointer obvs
    surface = IMG_Load("./penis.jpg");

    if (!surface) goto destruction;

    // Load image into GPU (or GPU allocated) memory
    tex = SDL_CreateTextureFromSurface(rend, surface);

    // Struct to hold the position of a spriate
    SDL_Rect dest;

    // Get dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 4;
    dest.h /= 4;

    Player player = {
        .position  = {  .x = 3.0f,  .y = 3.0f },
        .velocity  = { .dx = 0.0f, .dy = 0.0f },
        .direction = {  .x = 1.0f,  .y = 0.0f },
        .plane     = {  .x = 0.0f,  .y = 0.7f }
    };

    log_info("Player constructed:\n%s", display_player(&player));

    int32_t vert = 0;
    int32_t hoz = 0;
    int32_t turn = 0;

    // To allow the closing of the program
    bool close_requested = false;

    // Game loop
    while (!close_requested) {
        //creates the new event
        SDL_Event event;
        SDL_RenderClear(rend);

        // Poll events.
        while (SDL_PollEvent(&event)) {
            // Check for the type of the event
            switch (event.type) {
                case SDL_QUIT:
                    close_requested = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                            vert = 1;
                            break;
                        case SDL_SCANCODE_S:
                            vert = -1;
                            break;
                        case SDL_SCANCODE_D:
                            hoz = 1;
                            break;
                        case SDL_SCANCODE_A:
                            hoz = -1;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            turn = 1;
                            break;
                        case SDL_SCANCODE_LEFT:
                            turn = -1;
                            break;
                    }
                    log_info("KEYDOWN: %s.", display_position(&player.position));
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_S:
                            vert = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_A:
                            hoz = 0;
                            break;
                        case SDL_SCANCODE_RIGHT:
                        case SDL_SCANCODE_LEFT:
                            turn = 0;
                    }
                    break;
            }
        }

        // Ray position and direction
        for (uint32_t i = 0; i <= WINDOW_WIDTH; i++) {
            // Length of rays, lambda is the scalar that
            // essentially moves the ray across the camera plane
            float64_t lambda = (2 * i) / WINDOW_WIDTH - 1;
            float64_t ray_x =  player.direction.x + player.plane.x * lambda;
            float64_t ray_y = player.direction.y + player.plane.y * lambda;

            int32_t map_x = (int32_t)player.position.x;
            int32_t map_y = (int32_t)player.position.y;

            // Distance from the player to the first x or y side
            float64_t init_x, init_y;

            // Length from one x to the next
            float64_t delta_x = sqrt(1 + (ray_y * ray_y) / (ray_x * ray_x));
            float64_t delta_y = sqrt(1 + (ray_x * ray_x) / (ray_y * ray_y));
            // Normalise it, only the ratio matters (since it's a gradient)
            float64_t normal = 1/sqrt(ray_x*ray_x + ray_y*ray_y);
            delta_x *= normal;
            delta_y *= normal;

            // Step in either x or y, positive or negative,
            // depending on the direction of the ray (+-)
            int8_t step_x, step_y;

            bool hit = false;
            bool side; //x-side 0, y-side 1

            // Calculate the inital distance to the first side
            float64_t mu_x = player.position.x - map_x;
            float64_t mu_y = player.position.y - map_y;

            // Calculate the step size needed
            if (ray_x < 0) {
                step_x = -1;
                init_x = mu_x * delta_x;
            } else {
                step_x = 1;
                init_x = (1 - mu_x) * delta_x;
            }
            if (ray_y < 0) {
                step_y = -1;
                init_y = mu_y * delta_y;
            } else {
                step_y = 1;
                init_y = (1 - mu_y) * delta_y;
            }

            while (!hit) {
                if (init_x < init_y) {
                    init_x += delta_x;
                    map_x += step_x;
                    side = false;
                } else {
                    init_y += delta_y;
                    map_y += step_y;
                    side = true;
                }
                if (MAP[map_x][map_y] > 0) hit = true;
            }
            // Distance to the wall that was it, perp to the
            // camera plane, stops fisheye add one sometimes,
            // like above to correct position, think about it,
            // rest is basic geometry.

            float64_t perp_dist;
            if (side == false) perp_dist = (((1 - step_x) / 2) - mu_x) / ray_x;
            else               perp_dist = (((1 - step_y) / 2) - mu_y) / ray_y;

            // Line height varies inversely with distance from camera plane
            int32_t line_height = (int32_t) WINDOW_HEIGHT / perp_dist;

            // Calculate height of walls
            int32_t start = (WINDOW_HEIGHT - line_height) / 2;
            if (start < 0) start = 0;
            int32_t stop = (WINDOW_HEIGHT + line_height) / 2;
            if (stop >= WINDOW_HEIGHT) stop = WINDOW_HEIGHT - 1;

            switch (MAP[map_x][map_y]) {
                case 1:
                    SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
                    if (side)
                        SDL_SetRenderDrawColor(rend, 0, 0, 125, SDL_ALPHA_OPAQUE);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
                    if (side)
                        SDL_SetRenderDrawColor(rend, 0, 125, 0, SDL_ALPHA_OPAQUE);
                    break;
            }
            SDL_RenderDrawLine(rend, i, start, i, stop);
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

        // Movement system
        player.position.x += player.direction.x * hoz;
        player.position.y += player.direction.y * vert;

        // Rotation
        float64_t rot_speed = 1;

        float64_t old_dir_x = player.direction.x;
        player.direction.x =
              player.direction.x * cos(-rot_speed*turn)
            - player.direction.y * sin(-rot_speed*turn);
        player.direction.y =
                       old_dir_x * sin(-rot_speed*turn)
            + player.direction.y * cos(-rot_speed*turn);

        float64_t old_pln_x = player.plane.x;
        player.plane.x =
              player.plane.x * cos(-rot_speed*turn)
            - player.plane.y * sin(-rot_speed*turn);
        player.plane.y =
                   old_pln_x * sin(-rot_speed*turn)
            + player.plane.y * cos(-rot_speed*turn);

        SDL_RenderPresent(rend); // For float64_t buffering.

        SDL_Delay(10000/24);
    } // End of game loop


destruction:
    // Clean up stuff before exit

    if (surface) SDL_FreeSurface(surface);
    else log_fatal("Could not construct surface");

    if (tex) SDL_DestroyTexture(tex);
    else log_fatal("Failed to construct texture (`tex`).");

    if (rend) SDL_DestroyRenderer(rend);
    else log_fatal("Failed to initialise renderer (`rend`).");

    if (win) SDL_DestroyWindow(win);
    else log_fatal("Failed to get window (`win`).");

    log_warn("Quiting SDL and exiting application.");
    SDL_Quit();

    if (!(tex && rend && win))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
