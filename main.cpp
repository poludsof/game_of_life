#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

void get_coordinate(int index, int *x, int *y) {
    *y = 0;
    while (index > 40) {
        index -= 40;
        ++(*y);
    }
    *x = index;
}

void draw_square(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 );
    SDL_Rect rect = {(x * 20) + 1, (y * 20) + 1, 19, 19};
    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
    SDL_RenderFillRect( renderer, &rect);
}

void draw_pole(SDL_Renderer *render, int *pole) {
    int x, y;
    x = 0;
    y = 0;
    pole[10] = 1;
    pole[50] = 1;
    for(int i = 0; i < 1040; ++i) {
        if (pole[i] == 1) {
            get_coordinate(i, &x, &y);
            draw_square(render, x, y);
        }
    }
}

int main() {
    int pole[1040] = { 0 };

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

        draw_pole(renderer, pole);

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
