#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

extern int display[64*32];
extern uint8_t keypad[16];

//need to implement sound later xd
extern uint8_t stimer;

int scale = 10;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event sdl_e;

void Display_Init(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Shit8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64*scale, 32*scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_RenderSetScale(renderer, scale,scale);
}

int sdlkeys[16] = {SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a, SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v};

void Display(){
    while (SDL_PollEvent(&sdl_e)) {
        if(sdl_e.type == SDL_QUIT){
            exit(0);
        }else{
            if(sdl_e.type == SDL_KEYDOWN){
                for(int i = 0; i<16; i++){
                    if(sdl_e.key.keysym.sym == sdlkeys[i]){
                        keypad[i] = 1;
                    }
                }
            }else if(sdl_e.type == SDL_KEYUP){
                for(int i = 0; i<16; i++){
                    if(sdl_e.key.keysym.sym == sdlkeys[i]){
                        keypad[i] = 0;
                    }
                }
            }
        }
    }

    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            if(display[y*64+x] != 0){
                SDL_SetRenderDrawColor(renderer,0,0,0,1);
				SDL_RenderDrawPoint(renderer, x,y);
            }else{
                SDL_SetRenderDrawColor(renderer,131,176,126,1);
				SDL_RenderDrawPoint(renderer, x,y);
            }
        }
    }
    SDL_RenderPresent(renderer);
}