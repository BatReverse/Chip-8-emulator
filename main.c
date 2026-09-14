#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

int main(void) {
    printf("Hello, World!\n");

    // Au début de ton code
    const int SCALE = 15; // Chaque "pixel" du CHIP-8 fera 15x15 pixels à l'écran
    const int SCREEN_WIDTH = 64 * SCALE;  // Donne 960
    const int SCREEN_HEIGHT = 32 * SCALE; // Donne 480

    SDL_Window* window = SDL_CreateWindow("Chip-8 emu",
            SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH,SCREEN_HEIGHT,0);
    if (!window){
        printf("failed to create window\n");
        exit(EXIT_FAILURE);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("failed to create renderer\n");
        exit(EXIT_FAILURE);
    }
    
    SDL_Rect  rect_screen[64*32];
    for(int i=0; i<64*32;i++){
        rect_screen[i].w = SCALE;
        rect_screen[i].h = SCALE;

        rect_screen[i].x = (SCREEN_WIDTH/64)*(i%64);
        rect_screen[i].y = (SCREEN_HEIGHT/32)*(i/64);
    }

    bool quit = false;
    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        // User requests quit
        if(e.type == SDL_QUIT)
        {
            quit = true;
        }

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear screen
        SDL_RenderClear(renderer);

        // Set renderer color red to draw the square
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

        SDL_RenderFillRects(renderer,rect_screen,64*32);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    return 0;
}
