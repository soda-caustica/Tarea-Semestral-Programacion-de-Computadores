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
    tab tablero = readBoard(map,&pacman,&ghostNumber);
    ghost* ghosts = getGhosts(&tablero, ghostNumber);
    gameWindow game = createGame("oli",sprites,tablero);

    SDL_SetEventFilter(myKeyFilter,NULL);
    while(1){
        int exit = gameLoop(&game,&tablero,&pacman,ghosts,ghostNumber);
        if(exit != 0){
            break;
        } else {
            game.TPFp -= 21;
            resetGame(&tablero, &pacman, ghosts, ghostNumber);
        }

    }

    killGame(&game);
    killBoard(&tablero);

}

int gameLoop(gameWindow* game, tab* tablero, pacman* pacman, ghost* ghosts, int ghostNumber){
    while(1){
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
        if(pacman->points >= tablero->maxPoints/2 && !hasPointsLeft(tablero)){
            return 0;
        }
    }
}
