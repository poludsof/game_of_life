#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<unistd.h>

void draw_square(SDL_Renderer *renderer, int x, int y) {
//    std::cout << "draw " << x << " " << y << std::endl;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 );
    SDL_Rect rect = {(x * 20) + 1, (y * 20) + 1, 19, 19};
    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
    SDL_RenderFillRect( renderer, &rect);
}

int get_number_of_neighbors(int x, int y, const int pole[40][26]) {
    int count = 0;
    if (x != 0) {
        if (pole[x - 1][y] == 1) { // left neighbor
            ++count;
        }

        if (y != 0) {  // up left neighbor
            if (pole[x - 1][y - 1] == 1) {
                ++count;
            }
        }

        if (y != 25) { // down left neighbor
            if (pole[x - 1][y + 1] == 1) {
                ++count;
            }
        }
    }

    if (x != 39) {
        if (pole[x + 1][y] == 1) { // right neighbor
            ++count;
        }

        if (y != 0) {
            if (pole[x + 1][y - 1] == 1) { // up right neighbor
                ++count;
            }
        }

        if (y != 25) {
            if (pole[x + 1][y + 1] == 1) { // down right neighbor
                ++count;
            }
        }
    }

    if (y != 0) {
        if (pole[x][y - 1] == 1)
            ++count;
    }
    if (y != 25) {
        if (pole[x][y + 1] == 1)
            ++count;
    }

    return count;
}

void draw_pole(SDL_Renderer *render, const int pole[][26]) {
    for (int x = 0; x < 40; ++x) {
        for (int y = 0; y < 26; ++y) {
            if (pole[x][y] == 1) {
                draw_square(render, x, y);
            }
        }
    }
}

void create_copy_array(int pole[40][26], int copy_pole[40][26]) {
    for (int x = 0; x < 40; ++x) {
        for (int y = 0; y < 26; ++y) {
            copy_pole[x][y] = pole[x][y];
        }
    }
}

void update_field(int pole[40][26]) {
    int N;
    int copy_pole[40][26];
    create_copy_array(pole, copy_pole);

    for (int x = 0; x < 40; ++x) {
        for (int y = 0; y < 26; ++y) {
            N = get_number_of_neighbors(x, y, copy_pole);
            if (N < 2) { pole[x][y] = 0; }
            if (N > 3) { pole[x][y] = 0; }
            if (N == 3) {
                pole[x][y] = 1;
            }
//            if (N != 0) {
//                std::cout << x << " : " << y << " N: " << N << std::endl;
//            }
        }
    }
}

void get_up_left_index(int *x, int *y) {
    *x /= 20;
    *y /= 20;
}

int main() {
    int pole[40][26] = { 0 };

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("Game of Life",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       801, 521, 0);
    SDL_Event windowEvent;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    pole[2][2] = 1;
    pole[3][2] = 1;
    pole[4][2] = 1;

//    pole[5][2] = 1;
    pole[2][5] = 1;
    pole[3][5] = 1;
    pole[4][5] = 1;
    pole[5][5] = 1;

    auto start_game = true;

    while (true) {
        if (SDL_PollEvent(&windowEvent)) {
            if (SDL_QUIT == windowEvent.type)
                break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // grid
        SDL_SetRenderDrawColor(renderer, 152, 195, 220, 255);
        for (int i = 0; i <= 800; i += 20)
            SDL_RenderDrawLine(renderer, i, 0, i, 520);
        for (int i = 0; i <= 520; i+= 20)
            SDL_RenderDrawLine(renderer, 0, i, 800, i);

        if (start_game) {
            if (SDL_MOUSEBUTTONDOWN == windowEvent.type) {
                if (SDL_BUTTON_LEFT == windowEvent.button.button) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    get_up_left_index(&x, &y);
                    std::cout << x << " : " << y << std::endl;
                    pole[x][y] = 1;
                }
                if (SDL_BUTTON_RIGHT == windowEvent.button.button) {
                    start_game = false;
                }
            }
        }
        else {
            update_field(pole);
            sleep(1);
//            SDL_Delay(500);
        }
        draw_pole(renderer, pole);

        SDL_RenderPresent(renderer);

    }
    std::cout << "the end" << std::endl;

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
