#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "structs.h"
#include "gameFuncs.h"

//lee el archivo que guarda el mapa y almacena sus datos
tab readBoard(char* filepath, pacman* pac, int* ghosts){

    int ncolumns, nrows, maxPoints = 0;
    FILE* file = fopen(filepath,"r");

    if(file == NULL){
        tab tablero = {NULL,0,0,0};
        return tablero;
    }
    fscanf(file,"%d %d %d",&nrows, &ncolumns, ghosts);

    //guardamos el tablero y una copia de este para cuando reiniciemos el nivel
    int **board = malloc(nrows * sizeof(int*));
    int **copy = malloc(nrows * sizeof(int*));
    
    for(int i = 0; i < nrows; i++){
        board[i] = malloc(ncolumns * sizeof(int));
        copy[i] = malloc(ncolumns*sizeof(int));
    }
    
    //si encontramos el pacman, guardamos sus datos en el struct correspondiente, y aumentamos los puntos "maximos" del mapa (aunque los fantasmas pueden dar aun mas puntos)

    for(int i = 0; i < nrows; i++)
        for(int j = 0; j < ncolumns; j++){
            fscanf(file, "%d", &board[i][j]);
            copy[i][j] = board[i][j];

            switch(board[i][j]){  
                case CHARACTER:
                    pac->spawn_y = j;
                    pac->spawn_x = i;
                    break;
                case PELLET:
                    maxPoints += 10;
                    break;
                case POWER_PILL:
                    maxPoints += 50;
                    break;
            }
        }

    pac->x = pac->spawn_x;
    pac->y = pac->spawn_y;
    fclose(file);
    
    //retornamos los datos conseguidos como un struct de tablero
    tab tablero = {board,copy,nrows,ncolumns,maxPoints};
    return tablero;
}


//esta funcion encuentra los fantasmas del tablero
ghost* getGhosts(tab* board, int ghostNumber){

    int counter = 0;
    ghost* ghosts = malloc(ghostNumber*sizeof(ghost));

    //viajamos por la matriz, y cada vez que encontremos un fantasma guardamos sus datos
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
    
    //actualizamos el contador del power up del pacman
    if(pac->boosted > 0){
            pac->boosted -= 1;
    }
    

    //quitamos al pacman y a los fantasmas del mapa y manejamos su movimiento de forma interna, sin actualizar la matriz
    int** matrix = board->tabMat;
    clearBoard(board, ghosts, ghostNumber);
    movePacman(board, pac);
    moveGhosts(board,ghosts,ghostNumber);

    //revisamos si algun fantasma choco con pacman, retorna -1 si ninguno lo hizo
    int crasher = checkCollide(pac,ghosts,ghostNumber);

    if(crasher != -1){
        //manejamos la colision del pacman con el fantasma
        if(pacmanCollide(pac,&ghosts[crasher],board)){
            for(int i = 0; i < ghostNumber; i++){
                resetGhost(board,&ghosts[i]);
            }
            resetPacman(pac);
        }

    //actualizamos la matriz con los personajes para poder dibujarla
    matrix[pac->y][pac->x] = CHARACTER;
    for(int i = 0; i < ghostNumber; i++)
        matrix[ghosts[i].y][ghosts[i].x] = GHOST;
    }
    
}

//buscamos el pacman y los fantasmas en la matriz y los borramos
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

//obtenemos que esta pisando un fantasma especifico, ya que no aparece en la matriz porque lo cubre
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

    //manejamos los movimientos, eucmod es como el modulo, pero funciona con negativos, si se pasa de la matriz da la vuelta
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
   
    //si pisa una bolita o un potenciador le damos los puntos y/o el powerup, segun corresponda, y comprobamos si no esta tocando parec
    if(matrix[new_y][new_x] == PELLET){
        pac->points += 10;
        matrix[new_y][new_x] = SPACE;
    } else if (matrix[new_y][new_x] == POWER_PILL){
        pac->points += 50;
        pac->boosted = 10*1000/TPF;
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
    //esta funcion es similar a la del pacman, pero mas corta
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
    
    //esto verifica que no se esten moviendo a una pared o encima de otro fantasma
    if(matrix[new_y][new_x] != WALL && matrix[new_y][new_x] != GHOST){
        ghost->y = new_y;
        ghost->x = new_x;
        ghost->lastSteppedTile = matrix[new_y][new_x];
    }

}

    //para mover a cada fantasma de nuestro arreglo de fantasmas
void moveGhosts(tab* board, ghost* ghosts, int ghostNumber){
    for(int i = 0; i < ghostNumber; i++)
        if(rand() % 2 == 1){
            ghosts[i].direction = randDir();
            moveGhost(board, &ghosts[i]);
        }
}

    //en caso de atacar a pacman o de ser comido, esta funcion regresa a un fantasma a su casilla de aparicion
void resetGhost(tab* board,ghost* ghost){
    ghost->x = ghost->spawn_x;
    ghost->y = ghost->spawn_y;
    ghost->lastSteppedTile = board->tabMat[ghost->y][ghost->x];
}

    //en caso de ser atacado, esta funcion vuelve a pacman a su casilla de aparicion
void resetPacman(pacman* pac){
    pac->x = pac->spawn_x;
    pac->y = pac->spawn_y;
}

    //revisa entre todos los fantasmas si tienen la misma posicion que pacman
int checkCollide(const pacman* pac, const ghost* ghosts, int ghostNumber){
    for(int i = 0; i < ghostNumber; i++){
        ghost ghost = ghosts[i];
        if(ghost.x == pac->x && ghost.y == pac->y){
            return i;
        }
    }
    return -1;
}


    //maneja el evento de que pacman choque con un fantasma en especifico
int pacmanCollide(pacman* pac, ghost* ghost, tab* board){
    if(!pac->boosted){
        pac->lives -= 1;
        return 1;
    } else {
        resetGhost(board, ghost);
        pac->points += 100;
        return 0;
    }

}
    //en caso de acabarse los puntos, esta funcion agarra la copia del tablero, sobreescribe el tablero original y retorna todo el mapa a su estado original
    //igualmente retorna a los fantasmas y a pacman a sus posiciones originales
void resetGame(tab* board, pacman* pacman, ghost* ghosts, int ghostNumber){
    int** matrix = board->tabMat;
    int** backup = board->tabBackup;
    int columns = board->ncolumns, rows = board->nrows;
    
    matrixCopy(backup,matrix,rows,columns);

    pacman->x = pacman->spawn_x;
    pacman->y = pacman->spawn_y;
    pacman->boosted = 0;
    for(int i = 0; i < ghostNumber; i++){
        ghost ghost = ghosts[i];
        ghost.x = ghost.spawn_x;
        ghost.y = ghost.spawn_y;
    }

}

    //libera la memoria de un objeto de tablero, liberando los punteros de las 2 matrices
void killBoard(tab* board){
    int** matrix = board->tabMat;
    int** copy = board->tabBackup;
    int nrows = board-> nrows;
    
    for(int i = 0; i < nrows; i++)
        free(matrix[i]);
    free(matrix);

    for(int i = 0; i < nrows; i++)
        free(copy[i]);
    free(copy);
    
    board->tabMat = NULL;
    board->tabBackup = NULL;
    board->ncolumns = 0;
    board->nrows = 0;
}

    //itera sobre el tablero y revisa si quedan puntos por comer
int hasPointsLeft(tab* tablero){
    int** board = tablero->tabMat;
    for(int i = 0; i < tablero->nrows; i++){
        for(int j = 0; j < tablero->ncolumns; j++){
            if(board[i][j] == PELLET || board[i][j] == POWER_PILL){
                return 1;
            }
        }
    }
    return 0;
}

