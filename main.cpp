#include <iostream>
#include <SDL.h>

void draw_square(SDL_Renderer *renderer, int x, int y, int scale, int size, int r, int g, int b) {
    SDL_Rect rect = {(x * scale) + 1, (y * scale) + 1, size - 1, size - 1};
    SDL_SetRenderDrawColor( renderer, r, g, b, 255);
    SDL_RenderFillRect( renderer, &rect);
}

int get_number_of_neighbors(int x, int y, const int pole[40][26]) {
    int count = 0;
    if (x != 0) {
        if (pole[x - 1][y] == 1) { ++count; } // left neighbor

        if (y != 0) {  // up left neighbor
            if (pole[x - 1][y - 1] == 1) { ++count; }
        }

        if (y != 25) { // down left neighbor
            if (pole[x - 1][y + 1] == 1) { ++count; }
        }
    }

    if (x != 39) {
        if (pole[x + 1][y] == 1)  { ++count; } // right neighbor

        if (y != 0) {
            if (pole[x + 1][y - 1] == 1) { ++count; } // up right neighbor
        }

        if (y != 25) {
            if (pole[x + 1][y + 1] == 1) { ++count; } // down right neighbor
        }
    }

    if (y != 0) {
        if (pole[x][y - 1] == 1)  { ++count; }  // top neighbor
    }
    if (y != 25) {
        if (pole[x][y + 1] == 1) { ++count; }  // bottom neighbor
    }

    return count;
}

void draw_pole(SDL_Renderer *renderer, const int pole[][26], int r, int g, int b) {
    for (int x = 0; x < 40; ++x) {
        for (int y = 0; y < 26; ++y) {
            if (pole[x][y] == 1)
                draw_square(renderer, x, y, 20, 19, r, g, b);
        }
    }
}

void create_copy_array(const int pole[40][26], int copy_pole[40][26]) {
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
            if (N < 2) { pole[x][y] = 0; }     // the cell dies of loneliness
            if (N > 3) { pole[x][y] = 0; }     // the cell dies from overpopulation
            if (N == 3) { pole[x][y] = 1; }    // cell birth
        }
    }
}

void draw_color_menu(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 152, 195, 255, 255); // line color

    // vertical lines
    for (int i = 200; i <= 600; ++i) // up
        SDL_RenderDrawLine(renderer, i, 200, i, 201);
    for (int i = 200; i <= 600; ++i) // down
        SDL_RenderDrawLine(renderer, i, 280, i, 281);

    // horizontal lines
    for (int i = 201; i < 280; ++i) {
        for (int j = 200; j < 601; j += 80)
            SDL_RenderDrawLine(renderer, j, i, j + 1, i);
    }

    draw_square(renderer, 200, 200, 1, 80, 255, 20, 30);     // red
    draw_square(renderer, 281, 200, 1, 80, 10, 255, 45);     // green
    draw_square(renderer, 361, 200, 1, 80, 10, 120, 250);    // blue
    draw_square(renderer, 441, 200, 1, 80, 200, 255, 255);   // white
    draw_square(renderer, 521, 200, 1, 80, 240, 255, 50);    // yellow
}

bool select_color(SDL_Event windowEvent, int *r, int *g, int *b) {
    bool selected = false;
    int x, y;

    if (SDL_MOUSEBUTTONDOWN == windowEvent.type && SDL_BUTTON_LEFT == windowEvent.button.button) {
        SDL_GetMouseState(&x, &y);

        if (y > 200 && y < 280) {
            selected = true;

            if (x > 200 && x < 280) { *r = 255; *g = 20; *b = 30; }          // red
            else if (x > 281 && x < 360) { *r = 10; *g = 255; *b = 45; }     // green
            else if (x > 361 && x < 440) { *r = 10; *g = 120; *b = 250; }    // blue
            else if (x > 441 && x < 520) { *r = 200; *g = 255; *b = 255; }   // white
            else if (x > 522 && x < 600) { *r = 240; *g = 255; *b = 50; }    // yellow
            else selected = false;
        }
    }

    return selected;
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

    // game states
    bool drawing = false;
    bool start_game = false;
    bool choose_color = true;

    int r, g, b; // color of cells

    // menu with color selection
    while (choose_color) {
        SDL_SetRenderDrawColor(renderer, 3, 15, 25, 255); // background color
        SDL_RenderClear(renderer);

        draw_color_menu(renderer);

        SDL_RenderPresent(renderer);

        if (select_color(windowEvent, &r, &g, &b)) {
            drawing = true;
            choose_color = false;
        }

        if (SDL_PollEvent(&windowEvent) && SDL_QUIT == windowEvent.type) { break; }
    }

    SDL_Delay(700);

    while ((start_game || drawing)) {

        SDL_SetRenderDrawColor(renderer, 3, 15, 30, 255);
        SDL_RenderClear(renderer);

        // grid drawing on the background
        SDL_SetRenderDrawColor(renderer, 140, 180, 255, 255);
        for (int i = 0; i <= 800; i += 20)
            SDL_RenderDrawLine(renderer, i, 0, i, 520);
        for (int i = 0; i <= 520; i+= 20)
            SDL_RenderDrawLine(renderer, 0, i, 800, i);

        // drawing a world
        if (drawing) {
            if (SDL_PollEvent(&windowEvent) && SDL_MOUSEBUTTONDOWN == windowEvent.type) {

                if (SDL_BUTTON_LEFT == windowEvent.button.button) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    x /= 20; y /= 20;
                    pole[x][y] = 1;
                }

                // end drawing and start the game loop
                if (SDL_BUTTON_RIGHT == windowEvent.button.button) {
                    start_game = true;
                    drawing = false;
                }
            }
        }

        else if (start_game) {
            update_field(pole);  // updating the world
            SDL_Delay(500);
        }

        draw_pole(renderer, pole, r, g, b);

        if (SDL_PollEvent(&windowEvent) && SDL_QUIT == windowEvent.type) { break; }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
