#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include "structs.h"


tab readBoard(char* filepath, pacman* pacman, int* ghosts);
void moveGhost(tab* board, ghost* ghost, pacman* pacman, int ghostNumber);
void movePacman(tab* board, pacman* pacman, char newdir, ghost* ghosts, int ghostNumber);
void pacmanCollide(tab* tab, pacman* pacman, ghost* ghosts, int ghostNumber);
void ghostsReset(tab* tab, ghost* ghosts, int ghostNumber);
void moveGhosts(tab* tab, pacman* pacman,ghost* ghosts, int ghostNumber);
ghost* getGhosts(tab* board, int ghostNumber);
#endif
