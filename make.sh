#!/bin/bash
gcc -I/usr/include/SDL2 -lSDL2 -o main src/main.c src/vector.c src/graphics.c src/render.c -lm
