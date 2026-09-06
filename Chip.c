//
// Created by riri on 9/1/26.
//
#include "Chip.h"

#include <stdlib.h>
#include <string.h>

void initChip(Chip_t* chip) {
    memset(chip, 0, sizeof(Chip_t));
    chip->PC = 0x200;
}

void CLS(Chip_t* chip) {
    memset(chip->gfx, 0, 64*32);
}

void RET(Chip_t* chip) {
    chip->PC = chip->stack[chip->SP];
    chip->SP--;
}

void JP_addr(Chip_t* chip, uint16_t addr) {
    chip->PC = addr;
}

void CALL_addr(Chip_t* chip, uint16_t addr) {
    //faire attention a si la pile est pleine
    chip->SP++;
    chip->stack[chip->SP] = chip->PC;
    chip->PC = addr;
}

void SE_VX_byte(Chip_t* chip, uint8_t x, uint8_t kk) {
    if (chip->reg[x]==kk)
        chip->PC += 2;
}

void SNE_VX_byte(Chip_t* chip, uint8_t x, uint8_t kk) {
    if (chip->reg[x]!=kk)
        chip->PC += 2;
}

void SE_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    if (chip->reg[x]==chip->reg[y])
        chip->PC += 2;
}

void SNE_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    if (chip->reg[x]!=chip->reg[y])
        chip->PC += 2;
}

void LD_Vx_byte(Chip_t* chip, uint8_t x, uint8_t kk) {
    chip->reg[x]=kk;
}

void ADD_Vx_byte(Chip_t* chip, uint8_t x, uint8_t kk) {
    chip->reg[x]+=kk;
}

void LD_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]=chip->reg[y];
}

void OR_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]|=chip->reg[y];
}

void AND_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]&=chip->reg[y];
}

void XOR_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    chip->reg[x]^=chip->reg[y];
}

void ADD_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    if (chip->reg[y] + chip->reg[x] > 255)
        chip->reg[15] = 1;
    else
        chip->reg[15] = 0;
    chip->reg[x] += chip->reg[y];
}

void SUB_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    if (chip->reg[x]>chip->reg[y])
        chip->reg[15] = 1;
    else
        chip->reg[15] = 0;
    chip->reg[x] -= chip->reg[y];
}

void SHR_Vx(Chip_t* chip, uint8_t x) {
    chip->reg[15] = chip->reg[x] & 1;
    chip->reg[x]>>=1;
}

void SUBN_Vx_Vy(Chip_t* chip, uint8_t x, uint8_t y) {
    if (chip->reg[x] < chip->reg[y])
        chip->reg[15] = 1;
    else
        chip->reg[15] = 0;
    chip->reg[x] = chip->reg[y] - chip->reg[x];
}

void SHL_Vx(Chip_t* chip, uint8_t x) {
    chip->reg[15] = chip->reg[x] >> 7;
    chip->reg[x] <<= 1;
}

void LD_I_addr(Chip_t* chip, uint16_t addr) {
    chip->I = addr;
}

void RND_Vx_byte(Chip_t* chip, uint8_t x, uint8_t kk) {
    chip->reg[x] = rand() & kk;
}

void DRW_Vx_Vy_nibble(Chip_t* chip, uint8_t x, uint8_t y) {
    //todo
}




void fetch_decodeChip(Chip_t* chip) {
    const uint16_t opcode = (chip->ram[chip->PC] << 8) | chip->ram[chip->PC + 1];
    chip->PC += 2;

    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint16_t NNN = opcode & 0x0FFF;
    uint8_t NN = opcode & 0x00FF;

    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                CLS(chip);
            }if (opcode == 0x00EE) {
                RET(chip);
            }
            break;
        case 0x1000:
            JP_addr(chip, NNN);
            break;
        case 0x2000:
            CALL_addr(chip, NNN);

    }
}