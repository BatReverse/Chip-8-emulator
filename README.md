# Chip-8-emulator

A CHIP-8 emulator written in C, with SDL2 for display and input.

## Description

This project implements a CHIP-8 virtual machine: ROM loading, memory,
registers, stack, timers, as well as decoding and execution of the
instruction set. Display and keyboard input are handled through SDL2.

## Requirements

- A C compiler (gcc/clang) supporting C11
- SDL2 (`libsdl2-dev` on Debian/Ubuntu, `sdl2` on Arch/Fedora)
- `make` and `pkg-config`

## Build

```sh
make
```

This produces the `Chip_8` binary.

## Usage

```sh
./Chip_8 <path_to_rom>
```

## Project structure

- `Chip.h` / `Chip.c` — core of the emulator: registers, memory, stack,
  timers, ROM loading and opcode implementation
- `screen.h` / `screen.c` — SDL2 window, render loop and keyboard input
  handling
- `main.c` — program entry point
- `Makefile` — project build
- `TODO.md` — remaining tasks tracker

## Sources

- Tests: [chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
- Implementation reference: [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
