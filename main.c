#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "screen.h"


int main(void) {

    screen_t* screen = init(15);
    Chip_t* chip;
    initChip(chip);
    bool quit = false;
    // Event loop
    while(!quit)
    {
        quit = loop_it(screen,chip);
    }
    destroy_screen(screen);
    return 0;
}
