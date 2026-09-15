//
// Created by riri on 9/1/26.
//
#include "Chip.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FONTSET_ADDR 0x50

static const uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip_t* initChip() {
    Chip_t* chip = malloc(sizeof(Chip_t));
    memset(chip, 0, sizeof(Chip_t));
    chip->PC = 0x200;
    memcpy(chip->ram + FONTSET_ADDR, fontset, sizeof(fontset));
    return chip;
}

bool load_rom(Chip_t* chip,char* path){
    FILE* fptr;

    fptr = fopen(path,"rb");
    if(!fptr){
        printf("error opening file\n");
        return false;
    }
    int n=0;
    int byte;
    while ((byte=fgetc(fptr)) != EOF)
    {
        chip->ram[0x200+n] = (uint8_t)byte;
        n++;
        continue;
    }
    fclose(fptr);

    return true;
}


void OP_00E0(Chip_t* chip) {
    memset(chip->gfx, 0, 64*32);
}

void OP_00EE(Chip_t* chip) {
    chip->PC = chip->stack[chip->SP];
    chip->SP--;
}

void OP_1NNN(Chip_t* chip, uint16_t addr) {
    chip->PC = addr;
}

void OP_2NNN(Chip_t* chip, uint16_t addr) {
    //faire attention a si la pile est pleine
    chip->SP++;
    chip->stack[chip->SP] = chip->PC;
    chip->PC = addr;
}

void OP_3XKK(Chip_t* chip, uint8_t x, uint8_t kk) {
    if (chip->reg[x]==kk)
        chip->PC += 2;
}

void OP_4XKK(Chip_t* chip, uint8_t x, uint8_t kk) {
    if (chip->reg[x]!=kk)
        chip->PC += 2;
}

void OP_5XY0(Chip_t* chip, uint8_t x, uint8_t y) {
    if (chip->reg[x]==chip->reg[y])
        chip->PC += 2;
}

void OP_9XY0(Chip_t* chip, uint8_t x, uint8_t y) {
    if (chip->reg[x]!=chip->reg[y])
        chip->PC += 2;
}

void OP_6XKK(Chip_t* chip, uint8_t x, uint8_t kk) {
    chip->reg[x]=kk;
}

void OP_7XKK(Chip_t* chip, uint8_t x, uint8_t kk) {
    chip->reg[x]+=kk;
}

void OP_8XY0(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]=chip->reg[y];
}

void OP_8XY1(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]|=chip->reg[y];
    chip->reg[15] = 0; // quirk COSMAC VIP : les ops logiques resettent VF
}

void OP_8XY2(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]&=chip->reg[y];
    chip->reg[15] = 0;
}

void OP_8XY3(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]^=chip->reg[y];
    chip->reg[15] = 0;
}

void OP_8XY4(Chip_t* chip, uint8_t x, uint8_t y) {
    uint8_t vx = chip->reg[x];
    uint8_t vy = chip->reg[y];
    chip->reg[x] = vx + vy;
    chip->reg[15] = ((uint16_t)vx + vy > 255) ? 1 : 0; // flag écrit en dernier
}

void OP_8XY5(Chip_t* chip, uint8_t x, uint8_t y) {
    uint8_t vx = chip->reg[x];
    uint8_t vy = chip->reg[y];
    chip->reg[x] = vx - vy;
    chip->reg[15] = (vx >= vy) ? 1 : 0;
}

void OP_8XY6(Chip_t* chip, uint8_t x, uint8_t y) {
    uint8_t vy = chip->reg[y]; // quirk COSMAC VIP : décale VY, pas VX
    chip->reg[x] = vy >> 1;
    chip->reg[15] = vy & 1;
}

void OP_8XY7(Chip_t* chip, uint8_t x, uint8_t y) {
    uint8_t vx = chip->reg[x];
    uint8_t vy = chip->reg[y];
    chip->reg[x] = vy - vx;
    chip->reg[15] = (vy >= vx) ? 1 : 0;
}

void OP_8XYE(Chip_t* chip, uint8_t x, uint8_t y) {
    uint8_t vy = chip->reg[y];
    chip->reg[x] = vy << 1;
    chip->reg[15] = (vy >> 7) & 1;
}

void OP_ANNN(Chip_t* chip, uint16_t addr) {
    chip->I = addr;
}

void OP_CXKK(Chip_t* chip, uint8_t x, uint8_t kk) {
    chip->reg[x] = rand() & kk;
}

int coords_to_index(uint8_t x,uint8_t y){
    return x+64*y;
}

void OP_DXYN(Chip_t* chip, uint8_t x, uint8_t y,uint8_t n) {
    uint8_t Vx = chip->reg[x]%64;
    uint8_t Vy = chip->reg[y]%32;
    bool collision = false;
    for(uint8_t i = 0; i < n; i++){
        for(int j=0;j<8;j++){
            if(Vx+j >= 64 || Vy+i >= 32)
                continue;
            if(!collision && chip->gfx[coords_to_index(Vx+j,Vy+i)] && ((chip->ram[chip->I+i] >> (7-j)) & 1))
                collision = true;
            chip->gfx[coords_to_index(Vx+j,Vy+i)] ^= (chip->ram[chip->I+i] >> (7-j)) & 1;
        }
    } 
    chip->reg[15] = collision ? 1:0;
}

void OP_EX9E(Chip_t* chip, uint8_t x){
    if(chip->keypad[chip->reg[x]]){
        chip->PC += 2;
    }
}
void OP_EXA1(Chip_t* chip, uint8_t x){
    if(!chip->keypad[chip->reg[x]]){
        chip->PC += 2;
    }
}

void OP_FX07(Chip_t* chip,uint8_t x){
    chip->reg[x] = chip->DT;
}

void OP_FX0A(Chip_t* chip, uint8_t x){
    // comportement VIP : attend l'appui PUIS le relâchement d'une touche
    if (chip->fx0a_waiting_release) {
        if (!chip->keypad[chip->fx0a_key]) {
            chip->reg[x] = chip->fx0a_key;
            chip->fx0a_waiting_release = false;
        } else {
            chip->PC -= 2; // touche toujours enfoncée : on rejoue l'instruction
        }
        return;
    }

    for (uint8_t k = 0; k < 16; k++) {
        if (chip->keypad[k]) {
            chip->fx0a_key = k;
            chip->fx0a_waiting_release = true;
            break;
        }
    }
    chip->PC -= 2; // aucune touche pressée (ou en attente du relâchement) : on rejoue
}

void OP_FX15(Chip_t* chip, uint8_t x){
    //todo
    chip->DT = chip->reg[x];
}

void OP_FX18(Chip_t* chip,uint8_t x){
    chip->ST = chip->reg[x];
}

void OP_FX1E(Chip_t* chip,uint8_t x){
    chip->I += chip->reg[x];
}

void OP_BNNN(Chip_t* chip, uint16_t nnn){
    chip->PC = nnn+chip->reg[0];
}

void OP_FX29(Chip_t* chip, uint8_t x){
    chip->I = FONTSET_ADDR + chip->reg[x] * 5;
}

void OP_FX33(Chip_t* chip,uint8_t x){
    uint8_t v = chip->reg[x];
    //jamais au dessus de 255
    uint8_t hunderds = v/100;
    uint8_t tens = (v%100)/10;
    uint8_t ones = (v%10);

    chip->ram[chip->I] = hunderds;
    chip->ram[chip->I+1] = tens;
    chip->ram[chip->I+2] = ones;
}

void OP_FX55(Chip_t* chip,uint8_t x){
    uint16_t I = chip->I;
    for(int i=0;i<=x;i++){
        chip->ram[I+i] = chip->reg[i];
    }
    chip->I += x + 1; // quirk COSMAC VIP : I est incrémenté après Fx55/Fx65
}

//a ne pas confondre avec la commande précedente
void OP_FX65(Chip_t* chip,uint8_t x){
    uint16_t I = chip->I;
    for(int i=0;i<=x;i++){
        chip->reg[i] = chip->ram[I+i];
    }
    chip->I += x + 1;
}

void fetch_decodeChip(Chip_t* chip) {
    const uint16_t opcode = (chip->ram[chip->PC] << 8) | chip->ram[chip->PC + 1];
    chip->PC += 2;

    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint8_t N = opcode & 0x000F;
    uint16_t NNN = opcode & 0x0FFF;
    uint8_t NN = opcode & 0x00FF;

    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                OP_00E0(chip);
            }if (opcode == 0x00EE) {
                OP_00EE(chip);
            }
            break;
        case 0x1000:
            OP_1NNN(chip, NNN);
            break;
        case 0x2000:
            OP_2NNN(chip, NNN);
            break;
        case 0x3000:
            OP_3XKK(chip, X, NN);
            break;
        case 0x4000:
            OP_4XKK(chip, X, NN);
            break;
        case 0x5000:
            OP_5XY0(chip, X, Y);
            break;
        case 0x6000:
            OP_6XKK(chip, X, NN);
            break;
        case 0x7000:
            OP_7XKK(chip, X, NN);
            break;
        case 0x8000:
            switch (N) {
                case 0x0:
                    OP_8XY0(chip, X, Y);
                    break;
                case 0x1:
                    OP_8XY1(chip, X, Y);
                    break;
                case 0x2:
                    OP_8XY2(chip, X, Y);
                    break;
                case 0x3:
                    OP_8XY3(chip, X, Y);
                    break;
                case 0x4:
                    OP_8XY4(chip, X, Y);
                    break;
                case 0x5:
                    OP_8XY5(chip, X, Y);
                    break;
                case 0x6:
                    OP_8XY6(chip, X, Y);
                    break;
                case 0x7:
                    OP_8XY7(chip, X, Y);
                    break;
                case 0xE:
                    OP_8XYE(chip, X, Y);
                    break;
            }
            break;
        case 0x9000:
            OP_9XY0(chip, X, Y);
            break;
        case 0xA000:
            OP_ANNN(chip, NNN);
            break;
        case 0xB000:
            OP_BNNN(chip, NNN);
            break;
        case 0xC000:
            OP_CXKK(chip, X, NN);
            break;
        case 0xD000:
            OP_DXYN(chip, X, Y, N);
            break;
        case 0xE000:
            switch (NN) {
                case 0x9E:
                    OP_EX9E(chip, X);
                    break;
                case 0xA1:
                    OP_EXA1(chip, X);
                    break;
            }
            break;
        case 0xF000:
            switch (NN) {
                case 0x07:
                    OP_FX07(chip, X);
                    break;
                case 0x0A:
                    OP_FX0A(chip, X);
                    break;
                case 0x15:
                    OP_FX15(chip, X);
                    break;
                case 0x18:
                    OP_FX18(chip, X);
                    break;
                case 0x1E:
                    OP_FX1E(chip, X);
                    break;
                case 0x29:
                    OP_FX29(chip, X);
                    break;
                case 0x33:
                    OP_FX33(chip, X);
                    break;
                case 0x55:
                    OP_FX55(chip, X);
                    break;
                case 0x65:
                    OP_FX65(chip, X);
                    break;
            }
            break;
    }
}