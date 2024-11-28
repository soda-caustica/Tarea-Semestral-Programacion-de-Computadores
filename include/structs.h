#ifndef STRUCT_H
#define STRUCT_H

#include <SDL2/SDL.h>

//guarda el tablero y sus dimensiones
typedef struct{
    int** tabMat;
    int** tabBackup;
    int nrows, ncolumns;
    int maxPoints;
}tab;

// las estructuras de pacman y ghost son parecidas, guardan posicion y ultima direccion, solo que cada uno tiene variables propias que se usan mas adelante
typedef struct{
    int x, y;
    int spawn_x, spawn_y;
    int color;
    char direction;
    int lastSteppedTile;
} ghost;

typedef struct{
    int x, y;
    int spawn_x, spawn_y;
    char direction;
    int boosted;
    int points;
    int lives;
} pacman;

//guarda la ventana actual, su Surface para usar sprites, la hoja de sprites correspondiente y una variable que maneja el cierre en una estructura, junto con el contador animationCycle y el tiempo que se espera entre frames
typedef struct{
    SDL_Window* window;
    SDL_Surface* winSurface;
    SDL_Surface* spritesheet;
    char quitStatus;
    int animationCycle;
    int TPFp;
} gameWindow;

#endif

