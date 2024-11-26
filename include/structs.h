#ifndef STRUCT_H
#define STRUCT_H

#include <SDL2/SDL.h>

//guarda el tablero y sus dimensiones
typedef struct{
    int** tabMat;
    int nrows;
    int ncolumns;
} tab;

// las estructuras de pacman y ghost son parecidas, guardan posicion y ultima direccion, solo que el pacman guarda su punto de aparicion y el fantasma su color
typedef struct{
    int x;
    int y;
    int spawn_x;
    int spawn_y;
    int color;
    char direction;
    int lastSteppedTile;
} ghost;

typedef struct{
    int x;
    int y;
    int spawn_x;
    int spawn_y;
    char direction;
    int boosted;
    int points;
} pacman;

//guarda la ventana actual, su Surface para usar sprites, la hoja de sprites correspondiente y una variable que maneja el cierre en una estructura
typedef struct{
    SDL_Window* window;
    SDL_Surface* winSurface;
    SDL_Surface* spritesheet;
    char quitStatus;
    int animationCycle;
} gameWindow;

#endif

