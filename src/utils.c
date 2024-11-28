#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "structs.h"

// el operador de resto, pero no retorna negativos
int eucmod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

// da una direccion aleatoria
char randDir(){
    char dirs[] = {'W','A','S','D'};
    int n = rand();
    return dirs[n%4];
}

//imprime una matriz, esta para propositos de testeo
void printMatrix(const tab* board){
    int** matrix = board->tabMat;

    for(int i = 0; i < board->nrows;i++){
        for(int j = 0; j < board->ncolumns;j++)
            printf("%d ", matrix[i][j]);
        printf("\n");    
    }
    printf("----------------------------\n");
}

//copia una matriz src a una matriz dest, de fxc ambas
void matrixCopy(int** src, int** dest, int f, int c){
    for(int i = 0; i < f; i++)
        for(int j = 0; j < c; j++)
            dest[i][j] = src[i][j];

}


