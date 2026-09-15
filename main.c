#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "screen.h"



int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <rom_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    screen_t* screen = init(15);
    Chip_t* chip = initChip();
    printf("Chip init succesfully\n");
    if(!load_rom(chip,argv[1]))
        exit(EXIT_FAILURE);
    printf("Rom loaded succesfully \n");
    bool quit = false;
    // Event loop
    while(!quit)
    {
        quit = loop_it(screen,chip);
    }
    destroy_screen(screen);
    return 0;
}
