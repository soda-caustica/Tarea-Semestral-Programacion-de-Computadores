#ifndef SDLFUNCS_H
#define SDLFUNCS_H

#include <SDL2/SDL.h>
#include "structs.h"

#define TPF 750


int myKeyFilter(void* userdata, SDL_Event* event);
gameWindow createGame(char* name, char* spritePath, tab board);
void blitBoard(tab board, const gameWindow* game, const pacman* pacman, const ghost* ghosts, int ghostNumber);
void killGame(gameWindow* game);
char pollKey();
#endif
