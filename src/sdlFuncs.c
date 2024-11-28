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

    SDL_Window* window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,RES*board.ncolumns,RES*(board.nrows+2),SDL_WINDOW_SHOWN);

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

    gameWindow game = {window,winSurface,spritesheet,0,0,TPF};

    return game;
}


//Esta funcion recibe un tablero y una ventana y dibuja todos los elementos del mapa
void blitBoard(tab board, const gameWindow* game, const pacman* pacman, const ghost* ghosts, int ghostNumber){    

    int** matrix = board.tabMat;
    int rows = board.nrows;
    int columns = board.ncolumns;

    SDL_Surface* sheet = game->spritesheet;
    SDL_Surface* surface = game->winSurface;
    SDL_Window* window = game->window;

    //fromRect indica nuestra posicion en la hoja de sprites
    //toRect indica una posicion en la pantalla
    SDL_Rect fromRect = {0,0,RES,RES};
    SDL_Rect toRect = {0,0,RES,RES};

    //dibujamos el 99% del tablero, obteniendo cada casilla de la matriz, correspondiendola a un sprite de nuestra hoja, y copiando el sprite en la pantalla
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < columns; j++){
            switch(matrix[i][j]){
                case WALL:
                    fromRect.x = 0;
                    fromRect.y = 0;
                    break;
                case PELLET:
                    fromRect.x = RES*2;
                    fromRect.y = 0;
                    break;
                case POWER_PILL:
                    fromRect.x = RES*3;
                    fromRect.y = 0;
                    break;
                default:
                    fromRect.x = RES;
                    fromRect.y = 0;
                    break;
            }
            toRect.y = (i+1)*RES;
            toRect.x = (j)*RES;
            SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
        }

    // dibujamos el pacman y los fantasmas por separado, en el caso del pacman el sprite dependera de su direccion
    getPacmanSprite(pacman,&fromRect, game->animationCycle);

    toRect.x = (pacman->x)*RES;
    toRect.y = (pacman->y+1)*RES;

    SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
    
    //iteramos el arreglo de fantasmas, dibujando cada fantasma
    for(int i = 0; i < ghostNumber; i++){
        
        ghost ghost = ghosts[i];
        
        if(pacman->boosted == 0){
            fromRect.x = ghost.color*RES;
            fromRect.y = RES;
        } else {
            fromRect.x = 4*RES;
            fromRect.y = RES*(2 + game->animationCycle % 2 );
        }
        
        toRect.x = (ghost.x)*RES;
        toRect.y = (ghost.y+1)*RES;

        SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
    }

    //dibujamos la puntuacion!!
    //estas lineas solo obtienen el sprite en la hoja y la posicion del texto SCORE en la pantalla

    fromRect.x = 0;
    fromRect.y = 4*RES;
    fromRect.w = 2*RES;
    fromRect.h = RES;

    toRect.x = 0;
    toRect.y = 0;
    toRect.w = 4*RES;
    toRect.h = RES;

    SDL_BlitSurface(sheet, &fromRect, surface, &toRect);

    int score = pacman->points;
    
    toRect.w = RES/2;
    toRect.h = RES;
    //obtenemos cada digito de la puntuacion, buscamos el digito en nuestra hoja de sprites y lo copiamos en el lugar que corresponde
    //solo dibujamos 4 digitos para ser mas accesible a tableros mas pequeños
    for(int i = 1; i < 5; i++){
        if(score > 0){
            getNumberSprite(score%10,&fromRect);
            score /= 10;
        } else {
            fromRect.x = 4*RES;
            fromRect.y = 0;
        }
        toRect.x =+ 4*RES - i*RES/2;
        SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
    }
    
    //dibujamos las vidas
    int lives = pacman->lives;
    fromRect.w = RES;

    toRect.w = RES;
    toRect.x = 0;

    // por cada vida dibujamos un pacman, si no entonces dibujamos un cuadrado negro
    for(int i = 0; i < 4; i++){
        if(i < lives){
            fromRect.x = 0;
            fromRect.y = 3*RES;
        } else {
            fromRect.x = 4*RES;
            fromRect.y = 0;
        }
        toRect.x = i*RES;
        toRect.y = (rows+1)*RES;
        SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
    }

    //dibujamos los niveles
    //esto solo esta encontrando el texto LEVEL: en la hoja de sprites y el lugar donde va y copiando
    fromRect.x = 0;
    fromRect.y = 5*RES;
    fromRect.w = 2*RES;
    fromRect.h = RES;

    toRect.x = (columns-3)*RES;
    toRect.y = 0;
    toRect.w = 4*RES;
    toRect.h = RES;

    SDL_BlitSurface(sheet, &fromRect, surface, &toRect);

    //deducimos los niveles en base a que tan rapido va el juego
    int levels = (TPF - game->TPFp)/21 + 1;

    //lo mismo que para la puntuacion, obtenemos cada digito y lo dibujamos
    toRect.w = RES/2;
    toRect.h = RES;
    for(int i = 1; i < 3; i++){
        if(levels> 0){
            getNumberSprite(levels%10,&fromRect);
            levels /= 10;
        } else {
            fromRect.x = 4*RES;
            fromRect.y = 0;
        }
        toRect.x = (columns)*RES - i*RES/2;
        SDL_BlitSurface(sheet, &fromRect, surface, &toRect);
    }

    //actualizamos la pantalla para que nuestros cambios se vean reflejados
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
char pollKey(int tpf){
    int startTime = SDL_GetTicks();
    char lastKey = 'C';

    //mientras quede tiempo (reflejado por la variable timeLeft) esperamos algun evento de ingreso (previamente filtrados)
    while((SDL_GetTicks() - startTime) < tpf){
        SDL_Event event;

        // si no hay nada en la cola, saltamos a la siguiente iteracion con la esperanza que salga algo
        if(!SDL_PollEvent(&event)){
            continue;
        }
        
        //si el evento es un alt+f4, retornamos 'Q'
        if(event.type == SDL_QUIT){
            return 'Q';
        }

        //como nuestro filtro solo permite SDL_QUIT y eventos de tecla, entonces no hay que verificar mas
        //obtenemos la tecla presionada y su nombre, los comparamos con las teclas permitidas, y si son alguna de ellas entonces los guardamos en lastKey
        SDL_Keycode key = event.key.keysym.sym;
        const char* keyName = SDL_GetKeyName(key);
        
        if(strcmp(keyName,"W") && strcmp(keyName,"D") && strcmp(keyName,"S") && strcmp(keyName,"A") && strcmp(keyName,"Q")){
            continue;
        }
        
        lastKey = *keyName;
        
    }
    return lastKey;
}

//dado un pacman, vemos la hoja de sprites y obtenemos el sprite dependiendo de su direccion y un contador
//el contador es para que el pacman cicle entre 2 sprites, para que parezca que va comiendo
void getPacmanSprite(const pacman* pacman, SDL_Rect* rect, int animationCycle){
    int x = 0;
    int y = RES*(2 + animationCycle % 2);
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

//dado un numero de 1 digito, buscamos su posicion en la hoja de sprites y la guardamos en el puntero a un SDL_Rect
void getNumberSprite(int number, SDL_Rect* rect){
    int x = 2*RES, y = 4*RES, w = RES/2, h = RES;
    x += (number%5)*RES/2;
    y += (number/5)*RES;
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;

}

