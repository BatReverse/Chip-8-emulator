#include "screen.h"
#include <stdlib.h>
#include "Chip.h"

// Mapping clavier -> touches Chip-8 (0x0-0xF), par position physique (scancode).
// Layout Chip-8 :        Touches (position physique, AZERTY affiché) :
//   1 2 3 C                1 2 3 4
//   4 5 6 D                A Z E R
//   7 8 9 E                Q S D F
//   A 0 B F                W X C V
static int scancode_to_chip8_key(SDL_Scancode sc){
    switch(sc){
        case SDL_SCANCODE_1: return 0x1;
        case SDL_SCANCODE_2: return 0x2;
        case SDL_SCANCODE_3: return 0x3;
        case SDL_SCANCODE_4: return 0xC;
        case SDL_SCANCODE_Q: return 0x4; // A en AZERTY
        case SDL_SCANCODE_W: return 0x5; // Z en AZERTY
        case SDL_SCANCODE_E: return 0x6;
        case SDL_SCANCODE_R: return 0xD;
        case SDL_SCANCODE_A: return 0x7; // Q en AZERTY
        case SDL_SCANCODE_S: return 0x8;
        case SDL_SCANCODE_D: return 0x9;
        case SDL_SCANCODE_F: return 0xE;
        case SDL_SCANCODE_Z: return 0xA; // W en AZERTY
        case SDL_SCANCODE_X: return 0x0;
        case SDL_SCANCODE_C: return 0xB;
        case SDL_SCANCODE_V: return 0xF;
        default: return -1;
    }
}

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
    const int INSTRUCTIONS_PER_SECOND = 600;                // vitesse CPU réglable
    const double CYCLE_MS = 1000.0 / INSTRUCTIONS_PER_SECOND;
    const double TIMER_MS = 1000.0 / 60.0;                  // 60Hz fixe pour DT/ST

    static Uint32 last_frame_time = 0;
    static double cycle_accumulator = 0.0;
    static double timer_accumulator = 0.0;
    Uint32 now = SDL_GetTicks();

    if (last_frame_time == 0) last_frame_time = now;
    double elapsed = now - last_frame_time;
    last_frame_time = now;
    if (elapsed > 250.0) elapsed = 250.0; // évite un rattrapage massif après une pause (fenêtre minimisée...)
    cycle_accumulator += elapsed;
    timer_accumulator += elapsed;

    SDL_Event e;
    int N; //nombre de pixels qui sont noirs;

    // Traite tous les events en attente sans bloquer
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
        {
            return true;
        }
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            int key = scancode_to_chip8_key(e.key.keysym.scancode);
            if (key >= 0) {
                chip->keypad[key] = (e.type == SDL_KEYDOWN) ? 1 : 0;
            }
        }
    }

    // Cadence CPU : rattrape autant de cycles que nécessaire (pas juste un par frame)
    while (cycle_accumulator >= CYCLE_MS) {
        fetch_decodeChip(chip);
        cycle_accumulator -= CYCLE_MS;
    }

    // Cadence timers (60Hz)
    while (timer_accumulator >= TIMER_MS) {
        if (chip->DT > 0) chip->DT--;
        if (chip->ST > 0) chip->ST--;
        timer_accumulator -= TIMER_MS;
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

    SDL_Delay(1); // évite de monopoliser un cœur CPU en busy-wait

    return false;
}

void destroy_screen(screen_t* screen){
    free(screen->pixels);
    free(screen);
    screen = NULL;
}