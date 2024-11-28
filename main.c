#include "gameFuncs.h"
#include "sdlFuncs.h"
#include "structs.h"
#include "utils.h"
#include <time.h>

int main(int argc, char** argv){
    char* map = argv[1];
    char* sprites = argv[2];
    int ghostNumber = 0;
    pacman pacman = {0,0,0,0,'D',10,0,3};

    if (argc != 3){
        fprintf(stderr,"Uso: %s <mapa> <hoja de sprites>\n",argv[0]);
        return 1;
    }

    //inicializamos tablero, el arreglo de fantasmas y la ventana del juego
    tab tablero = readBoard(map,&pacman,&ghostNumber);
    ghost* ghosts = getGhosts(&tablero, ghostNumber);
    gameWindow game = createGame("oli",sprites,tablero);

    SDL_SetEventFilter(myKeyFilter,NULL);
    while(1){
        //manejamos las formas en las que puede terminar un nivel, 1 es game over, -1 es salida forzada y 0 es obtener todos los puntos del nivel
        int exit = gameLoop(&game,&tablero,&pacman,ghosts,ghostNumber);
        if(exit != 0){
            break;
        } else {
            game.TPFp -= 21;
            resetGame(&tablero, &pacman, ghosts, ghostNumber);
        }

    }

    //liberamos la memoria pedida al inicializar la ventana y el tablero
    killGame(&game);
    killBoard(&tablero);

}

int gameLoop(gameWindow* game, tab* tablero, pacman* pacman, ghost* ghosts, int ghostNumber){
    while(1){
        //dibujamos el tablero en pantalla, esperamos el input, actualizamos los personajes y manejamos las condiciones de salida
        blitBoard(*tablero,game,pacman,ghosts,ghostNumber);
        char c = pollKey(game->TPFp);
        if(c == 'Q'){
            return -1;
        } else if (c != 'C'){
            pacman->direction = c;
        }
        updateActors(tablero,pacman,ghosts,ghostNumber);
        game->animationCycle +=1;
        if(pacman->lives == 0){
            return 1;
        }

        //si tenemos tres cuartos de los puntos totales del mapa empezamos a verificar si faltan puntos por recolectar
        if(pacman->points >= 3*tablero->maxPoints/4 && !hasPointsLeft(tablero)){
            return 0;
        }
    }
}
