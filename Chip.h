//
// Created by riri on 9/1/26.
//

#ifndef CHIP_8_CHIP_H
#define CHIP_8_CHIP_H
#include <stdint.h>
#include <stdbool.h>

typedef struct Chip {
    uint8_t reg[16];
    uint8_t ram[4096];
    uint16_t I; //registre index
    uint16_t PC;
    uint16_t SP;//Stack pointer
    uint16_t stack[16];
    uint8_t DT; //delay_timer
    uint8_t ST; //sound_timer

    // Périphériques
    uint8_t gfx[64*32]; // Ecran noir ou blanc
    uint8_t keypad[16]; //Etat des 16 touches

    // Etat de Fx0A (attente d'une touche, bloquant sur plusieurs cycles)
    bool fx0a_waiting_release;
    uint8_t fx0a_key;
} Chip_t;
//reg[15] (VF) est un flag
Chip_t* initChip();
bool load_rom(Chip_t* chip,char* path);
void fetch_decodeChip(Chip_t* chip);
#endif //CHIP_8_CHIP_H
