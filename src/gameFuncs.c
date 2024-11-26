#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "structs.h"
#include "gameFuncs.h"

//lee el archivo que guarda el mapa y almacena sus datos
tab readBoard(char* filepath, pacman* pac, int* ghosts){

    int ncolumns, nrows;
    FILE* file = fopen(filepath,"r");

    if(file == NULL){
        tab tablero = {NULL,0,0};
        return tablero;
    }
    fscanf(file,"%d %d %d",&nrows, &ncolumns, ghosts);

    int **board = malloc(nrows * sizeof(int*));
    for(int i = 0; i < nrows; i++)
        board[i] = malloc(ncolumns * sizeof(int));

    for(int i = 0; i < nrows; i++)
        for(int j = 0; j < ncolumns; j++){
            fscanf(file, "%d", &board[i][j]);
            if(board[i][j] == CHARACTER){
                pac->spawn_y = j;
                pac->spawn_x = i;
            }
        }
    pac->x = pac->spawn_x;
    pac->y = pac->spawn_y;
    fclose(file);
    
    tab tablero = {board,nrows,ncolumns};
    return tablero;
}

ghost* getGhosts(tab* board, int ghostNumber){
    int counter = 0;
    ghost* ghosts = malloc(ghostNumber*sizeof(ghost));
    for(int i = 0; i < board->nrows; i++)
        for(int j = 0; j < board->ncolumns; j++)
            if(board->tabMat[i][j] == GHOST){
                ghosts[counter].x = j;
                ghosts[counter].y = i;
                ghosts[counter].spawn_x = j;
                ghosts[counter].spawn_y = i;
                ghosts[counter].direction = 'W';
                ghosts[counter].color = counter;
                ghosts[counter].lastSteppedTile = PELLET;
                counter++;
            }
    return ghosts;
}

void updateActors(tab* board, pacman* pac, ghost* ghosts, int ghostNumber){
    
    if(pacman.boosted > 0){
            pacman.boosted -= 1
    }
    
    int** matrix = board->tabMat;
    clearBoard(board, ghosts, ghostNumber);
    movePacman(board, pac);
    moveGhosts(board,ghosts,ghostNumber);

    int crasher = checkCollide(pac,ghosts,ghostNumber);

    if(crasher != -1){
        if(pacmanCollide(pac,&ghosts[crasher],board)){
            for(int i = 0; i < ghostNumber; i++){
                resetGhost(board,&ghosts[i]);
            }
            resetPacman(pac);
        }
    matrix[pac->y][pac->x] = CHARACTER;
    for(int i = 0; i < ghostNumber; i++)
        matrix[ghosts[i].y][ghosts[i].x] = GHOST;
    }
    
}

void clearBoard(tab* board, ghost* ghosts, int ghostNumber){
    int** matrix = board -> tabMat;
    int ncolumns = board-> ncolumns;
    int nrows = board-> nrows;

    for(int i = 0; i < nrows; i++)
        for(int j = 0; j < ncolumns; j++)
            if(matrix[i][j] == GHOST || matrix[i][j] == CHARACTER){
                matrix[i][j] = SPACE;
            }
}

int getTile(int x, int y, ghost* ghosts, int ghostNumber){
    for(int i = 0; i < ghostNumber; i++)
        if(ghosts[i].x == x && ghosts[i].y == y)
            return ghosts[i].lastSteppedTile;
    return SPACE;
}

void movePacman(const tab* board, pacman* pac){
    int** matrix = board->tabMat;
    int nrows = board->nrows;
    int ncolumns = board->ncolumns;
    int new_x = pac->x;
    int new_y = pac->y;

    switch(pac->direction){
        case 'D':
            new_x = eucmod(new_x + 1, ncolumns);
            break;
        case 'A':
            new_x = eucmod(new_x - 1, ncolumns);
            break;
        case 'S':
            new_y = eucmod(new_y + 1, nrows);
            break;
        case 'W':
            new_y = eucmod(new_y - 1, nrows);
            break;
    }
   
    if(matrix[new_y][new_x] == PELLET){
        pac->points += 10;
        matrix[new_y][new_x] = SPACE;
    }
    if(matrix[new_y][new_x] != WALL){
        pac->y = new_y;
        pac->x = new_x;
    }
}

void moveGhost(const tab* board, ghost* ghost){
    int** matrix = board->tabMat;
    int nrows = board->nrows;
    int ncolumns = board->ncolumns;
    int new_x = ghost->x;
    int new_y = ghost->y;

    switch(ghost->direction){
        case 'D':
            new_x = eucmod(new_x + 1, ncolumns);
            break;
        case 'A':
            new_x = eucmod(new_x - 1, ncolumns);
            break;
        case 'S':
            new_y = eucmod(new_y + 1, nrows);
            break;
        case 'W':
            new_y = eucmod(new_y - 1, nrows);
            break;
    }
    
    if(matrix[new_y][new_x] != WALL && matrix[new_y][new_x] != GHOST){
        ghost->y = new_y;
        ghost->x = new_x;
        ghost->lastSteppedTile = matrix[new_y][new_x];
    }

}

void moveGhosts(tab* board, ghost* ghosts, int ghostNumber){
    for(int i = 0; i < ghostNumber; i++)
        if(rand() % 2 == 1){
            ghosts[i].direction = randDir();
            moveGhost(board, &ghosts[i]);
        }
}

void resetGhost(tab* board,ghost* ghost){
    ghost->x = ghost->spawn_x;
    ghost->y = ghost->spawn_y;
    ghost->lastSteppedTile = board->tabMat[ghost->y][ghost->x];
}

void resetPacman(pacman* pac){
    pac->x = pac->spawn_x;
    pac->y = pac->spawn_y;
}

int checkCollide(const pacman* pac, const ghost* ghosts, int ghostNumber){
    for(int i = 0; i < ghostNumber; i++){
        ghost ghost = ghosts[i];
        if(ghost.x == pac->x && ghost.y == pac->y){
            return i;
        }
    }
    return -1;
}



int pacmanCollide(pacman* pac, ghost* ghost, tab* board){
    if(!pac->boosted){
        return 1;
    } else {
        resetGhost(board, ghost);
        pac->points += 500;
        return 0;
    }

}

void killBoard(tab* board){
    int** matrix = board->tabMat;
    int nrows = board-> nrows;
    for(int i = 0; i < nrows; i++)
        free(matrix[i]);
    free(matrix);
    board->tabMat = NULL;
    board->ncolumns = 0;
    board->nrows = 0;
}
