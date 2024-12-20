#ifndef UTILS_H
#define UTILS_H

//definimos todas nuestras variables fijas, TPF es el tiempo inicial por frame, este cambia dependiendo del nivel, 
//los otros son los enteros que representan cada tipo de casilla
#define TPF 250
#define RES 32
#define SPACE 0
#define CHARACTER 1
#define WALL 2
#define GHOST 3
#define PELLET 4
#define POWER_PILL 5
#include "structs.h"
#include <SDL2/SDL.h>

int eucmod(int a, int b);
char randDir();
void printMatrix(const tab* board);
void matrixCopy(int** src, int** dest, int f, int c);
int gameLoop(gameWindow* game, tab* tablero, pacman* pacman, ghost* ghosts, int ghostNumber);
#endif
