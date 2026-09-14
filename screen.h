#ifndef SCREEN
#define SCREEN

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct screen
{
    int SCALE;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    SDL_Rect* pixels; 
    SDL_Window* window;
    SDL_Renderer* renderer; 
} screen_t;


screen_t* init(int scale);
bool loop_it(screen_t* screen);


#endif