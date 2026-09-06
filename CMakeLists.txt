cmake_minimum_required(VERSION 4.3)
project(Chip_8 C)

set(CMAKE_C_STANDARD 11)

add_executable(Chip_8
        main.c
        Chip.h
        Chip.c)
