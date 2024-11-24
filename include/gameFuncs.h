#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include "structs.h"


tab readBoard(char* filepath, pacman* pac, int* ghosts);
ghost* getGhosts(tab* board, int ghostNumber);
void updateActors(tab* board, pacman* pac, ghost* ghosts, int ghostNumber);
void clearBoard(tab* board, ghost* ghosts, int ghostNumber);
int getTile(int x, int y, ghost* ghosts, int ghostNumber);
void movePacman(const tab* board, pacman* pac);
void moveGhost(const tab* board, ghost* ghost);
void moveGhosts(tab* board, ghost* ghosts, int ghostNumber);
void resetGhost(tab* board, ghost* ghost);
void resetPacman(pacman* pac);
int checkCollide(const pacman* pac, const ghost* ghosts, int ghostNumber);
int pacmanCollide(pacman* pac, ghost* ghost, tab* board);
void killBoard(tab* board);
#endif
