#ifndef UTILS_H
#define UTILS_H

#define RES 16
#define SPACE 0
#define CHARACTER 1
#define WALL 2
#define GHOST 3
#define PELLET 4
#include "structs.h"

int eucmod(int a, int b);
char randDir();
void printMatrix(const tab* board);
#endif