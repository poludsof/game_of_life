#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow("Game of Life",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       801, 521, 0);
    SDL_Event windowEvent;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    while (true) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 152, 195, 220, 255);
        for (int i = 0; i <= 800; i += 20) {
            SDL_RenderDrawLine(renderer, i, 0, i, 520);
        }
        for (int i = 0; i <= 520; i+= 20) {
            SDL_RenderDrawLine(renderer, 0, i, 800, i);
        }
        SDL_RenderPresent(renderer);

        if (SDL_PollEvent(&windowEvent)) {
            if (SDL_QUIT == windowEvent.type) {
                break;
            }
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
