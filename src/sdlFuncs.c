#include <SDL2/SDL.h>
#include "structs.h"
#include "utils.h"
#include "sdlFuncs.h"

// funcion para filtrar los eventos que nos importan de la fila de eventos con SDL_SetEventFilter
int myKeyFilter(void* userdata, SDL_Event* event){
    if(event->type == SDL_QUIT || event->type == SDL_KEYDOWN)
        return 1;
    
    return 0;
}

//recibe un tablero, un nombre y una direccion y crea una ventana con las dimensiones y la hoja de sprites correspondientes
gameWindow createGame(char* name, char* spritePath, tab board){

    SDL_Window* window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,RES*board.ncolumns,RES*(board.nrows+1),SDL_WINDOW_SHOWN);

    if (window == NULL){ 
        gameWindow game = {NULL,NULL,NULL,1};
        return game;
    }

    SDL_Surface* winSurface = SDL_GetWindowSurface(window);
    SDL_Surface* spritesheet = SDL_LoadBMP(spritePath);
    
    if(spritesheet == NULL){
        SDL_DestroyWindow(window);
        gameWindow game = {NULL,NULL,NULL,2};
        return game;
    }

    gameWindow game = {window,winSurface,spritesheet,0,0};

    return game;
}


//Esta funcion recibe un tablero y una ventana
void blitBoard(tab board, const gameWindow* game, const pacman* pacman, const ghost* ghosts, int ghostNumber){    

    int** matrix = board.tabMat;
    int rows = board.nrows;
    int columns = board.ncolumns;

    SDL_Surface* sheet = game->spritesheet;
    SDL_Surface* surface = game->winSurface;
    SDL_Window* window = game->window;

    SDL_Rect fromRect = {0,0,RES,RES};
    SDL_Rect toRect = {0,0,RES,RES};

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < columns; j++){
            if(matrix[i][j] == WALL){
                fromRect.x = 0;
                fromRect.y = 0;
            } else if(matrix[i][j] == PELLET){
                fromRect.x = RES*2;
                fromRect.y = 0;
            } else{
                fromRect.x = RES;
                fromRect.y = 0;
            }
            toRect.y = i*RES;
            toRect.x = j*RES;
            SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
        }

    // dibujamos el pacman y los fantasmas por separado
    getPacmanSprite(pacman,&fromRect, game->animationCycle);

    toRect.x = pacman->x*RES;
    toRect.y = pacman->y*RES;

    SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
    
    for(int i = 0; i < ghostNumber; i++){

        ghost ghost = ghosts[i];
        fromRect.x = ghost.color*RES;
        fromRect.y = RES;
        toRect.x = ghost.x*RES;
        toRect.y = ghost.y*RES;

        SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
    }
    SDL_UpdateWindowSurface(window);
}


// Esta funcion libera la memoria de la ventana de SDL
void killGame(gameWindow* game){

    SDL_FreeSurface(game->winSurface);
    game->winSurface = NULL;

    SDL_DestroyWindow(game->window);
    game->window = NULL;

    SDL_Quit();
}

//Espera el tiempo por frame por un ingreso, registrando el ultimo ingreso que cumpla las condiciones y devolviendolo, si no hay nada regresa "C"
char pollKey(){
    int startTime = SDL_GetTicks();
    int timeLeft;
    char lastKey = 'C';
    while((timeLeft = SDL_GetTicks() - startTime) < TPF){
        SDL_Event event;
        if(!SDL_PollEvent(&event)){
            continue;
        }
        
        if(event.type == SDL_QUIT){
            return 'Q';
        }

        SDL_Keycode key = event.key.keysym.sym;
        const char* keyName = SDL_GetKeyName(key);
        
        if(strcmp(keyName,"W") && strcmp(keyName,"D") && strcmp(keyName,"S") && strcmp(keyName,"A")){
            continue;
        }
        
        lastKey = *keyName;
        
    }
    return lastKey;
}

void getPacmanSprite(const pacman* pacman, SDL_Rect* rect, int animationCycle){
    int x = 0;
    int y = RES*(2 + animationCycle);
    switch(pacman->direction){
        case 'A':
            x = 0;
            break;
        case 'D':
            x = 1;
            break;
        case 'W':
            x = 2;
            break;
        case 'S':
            x = 3;
            break;
    }
    x = x*RES;

    rect->x = x;
    rect->y = y;
    
}


