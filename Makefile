#!/bin/bash
g++ -c src/*.cpp -I /usr/include/SDL2/
g++ -o bugship *.o -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
