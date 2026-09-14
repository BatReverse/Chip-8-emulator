#include "screen.h"
#include <stdlib.h>
#include "Chip.h"

screen_t* init(int scale){
    int SCREEN_WIDTH = 64*scale;
    int SCREEN_HEIGHT = 32*scale;

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

    screen_t* res = malloc(sizeof(screen_t));
    res->pixels = malloc(sizeof(SDL_Rect)*64*32);
    res->renderer = renderer;
    res->SCALE = scale;
    res->SCREEN_HEIGHT = SCREEN_HEIGHT;
    res->SCREEN_WIDTH = SCREEN_WIDTH;
    res->window = window;

    return res;

}

bool loop_it(screen_t* screen, Chip_t* chip){
    SDL_Event e;
    int N; //nombre de pixels qui sont noirs;

    // Wait indefinitely for the next available event
    SDL_WaitEvent(&e);

    // User requests quit
    if(e.type == SDL_QUIT)
    {
        return true;
    }
    
    N=0;
    for(int i = 0; i<64*32;i++){
        if(chip->gfx[i]){
            screen->pixels[N].w =screen->SCALE;
            screen->pixels[N].h =screen->SCALE;
            screen->pixels[N].x =screen->SCALE * (i%64);
            screen->pixels[N].y =screen->SCALE * (i/64);
            N++;
        }
    }

    // Initialize renderer color white for the background
    SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Clear screen
    SDL_RenderClear(screen->renderer);

    // Set renderer color red to draw the square
    SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);

    SDL_RenderFillRects(screen->renderer,screen->pixels,N);

    // Update screen
    SDL_RenderPresent(screen->renderer);
    return false;
}

void destroy_screen(screen_t* screen){
    free(screen->pixels);
    free(screen);
    screen = NULL;
}