#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "structs.h"
int eucmod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

char randDir(){
    char dirs[] = {'W','A','S','D'};
    int n = rand();
    return dirs[n%4];
}

void printMatrix(const tab* board){
    int** matrix = board->tabMat;

    for(int i = 0; i < board->nrows;i++){
        for(int j = 0; j < board->ncolumns;j++)
            printf("%d ", matrix[i][j]);
        printf("\n");    
    }
    printf("----------------------------\n");
}
