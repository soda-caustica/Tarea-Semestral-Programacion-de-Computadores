#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "structs.h"
#include "gameFuncs.h"

//lee el archivo que guarda el mapa y almacena sus datos
tab readBoard(char* filepath, pacman* pacman, int* ghosts){

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
                board[i][j] = SPACE;
                pacman->spawn_y = j;
                pacman->spawn_x = i;
            }
        }
    pacman->x = pacman->spawn_x;
    pacman->y = pacman->spawn_y;
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
                ghosts[counter].color = counter;
                ghosts[counter].lastSteppedTile = SPACE;
                counter++;
            }
    return ghosts;
}

//mueve un fantasma cualquiera a una direccion aleatoria
void moveGhost(tab* board, ghost* ghost, pacman* pacman, int ghostNumber, int i){
    int* x = &ghost->x;
    int* y = &ghost->y;
    int dx = *x, dy = *y;

    int** matriz = board->tabMat;
    int nrows = board->nrows;
    int ncolumns = board->ncolumns;
    char direccion = randDir();

    switch(direccion){
        case 'D':
            dx = eucmod(dx+1,ncolumns); 
            break;
        case 'A':
            dx = eucmod(dx-1,ncolumns);
            break;
        case 'S':
            dy = eucmod(dy+1,nrows);
            break;
        case 'W':
            dy = eucmod(dy-1,nrows);
            break;
    }


    if (matriz[dy][dx] == CHARACTER){
        pacmanCollide(board,pacman,ghosts,ghostNumber);
    }
    else if(matriz[dy][dx] != WALL && matriz[dy][dx] != GHOST){
        matriz[*y][*x] = ghost->lastSteppedTile;
        ghost->lastSteppedTile = matriz[dy][dx];
        matriz[dy][dx] = GHOST;
        *y = dy;
        *x = dx;
    }

}

void moveGhosts(tab* tab, pacman* pacman, ghost* ghosts, int ghostNumber){
    for(int i = 0; i < ghostNumber; i++)
        moveGhost(tab, &(ghosts[i]), pacman, ghostNumber);
}

void ghostsReset(tab* tab, ghost* ghosts, int ghostNumber){
    int** tablero = tab->tabMat;

    for(int i = 0; i < ghostNumber; i++){

        ghost ghost = ghosts[i];

        tablero[ghost.y][ghost.x] = ghost.lastSteppedTile;
        ghost.x = ghost.spawn_x;
        ghost.y = ghost.spawn_y;

        if(tablero[ghost.y][ghost.x] != CHARACTER){
            ghost.lastSteppedTile = tablero[ghost.y][ghost.x];
        } else {
            ghost.lastSteppedTile = SPACE;
        }
    }
}
//mueve el pacman a una direccion entregada y lee si pacman muere
void movePacman(tab* board, pacman* pacman, char newdir, ghost* ghosts, int ghostNumber){
    int** matrix = board->tabMat;
    int* x = &(pacman->x);
    int* y = &(pacman->y);
    int dx=*x, dy=*y;

    int nrows = board->nrows;
    int ncolumns = board->ncolumns;

    if(newdir == 'C')
        newdir = pacman->direction;

    switch(newdir){
        case 'D':
            dx = eucmod(*x+1,ncolumns); 
            break;
        case 'A':
            dx = eucmod(*x-1,ncolumns);
            break;
        case 'S':
            dy = eucmod(*y+1,nrows);
            break;
        case 'W':
            dy = eucmod(*y-1,nrows);
            break;
    }

    pacman->direction = newdir;
    if(matrix[dy][dx] == GHOST){
        pacmanCollide(board, pacman, ghosts, ghostNumber);
        
    } else if(board->tabMat[dy][dx] != WALL){
        matrix[*y][*x] = SPACE;
        matrix[dy][dx] = CHARACTER;
        *y = dy;
        *x = dx;
    }
    
}

void pacmanCollide(tab* tab, pacman* pacman, ghost* ghosts, int ghostNumber){
    if(!pacman->boosted){
        pacman->dead = 1;
        pacman->x = pacman->spawn_x;
        pacman->y = pacman->spawn_y;
        ghostsReset(tab,ghosts,ghostNumber);
        printf("AAAAAAAAAAA");
        return;
    }


}
