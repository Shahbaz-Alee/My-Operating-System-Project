#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TILE_SIZE 40
#define ROWS 10
#define COLS 10
#define MINES 10

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool board[ROWS][COLS]; // True if mine is present, false otherwise
bool revealed[ROWS][COLS]; // True if tile is revealed, false otherwise
int adjacentMines[ROWS][COLS]; // Number of adjacent mines for each tile

void revealTile(int y, int x);

// Function to initialize SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Function to close SDL
void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Function to render the game
void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect tileRect;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            tileRect.x = j * TILE_SIZE;
            tileRect.y = i * TILE_SIZE;
            if (revealed[i][j]) {
                SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                SDL_RenderFillRect(renderer, &tileRect);
                if (board[i][j]) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tileRect);
                } else {
                    char buffer[10];
                    sprintf(buffer, "%d", adjacentMines[i][j]);
                    SDL_Surface* surface = IMG_Load("number.png"); // Load a PNG image for the number
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect textRect = {tileRect.x + 10, tileRect.y + 10, 20, 20};
                    SDL_RenderCopy(renderer, texture, NULL, &textRect);
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
            } else {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                SDL_RenderDrawRect(renderer, &tileRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

// Function to handle events
void handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)!= 0) {
        if (e.type == SDL_QUIT) {
            closeSDL();
            exit(0);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x / TILE_SIZE;
            int y = e.button.y / TILE_SIZE;
            if (e.button.button == SDL_BUTTON_LEFT) {
                revealTile(y, x);
            } else if (e.button.button == SDL_BUTTON_RIGHT) {
                // Flag tile
                if (revealed[y][x]) {
                    revealed[y][x] = false;
                } else {
                    revealed[y][x] = true;
                }
            }
        }
    }
}

// Function to reveal a tile
void revealTile(int y, int x) {
    if (board[y][x]) {
        // Game over
        printf("Game over!\n");
        closeSDL();
        exit(0);
    } else {
        revealed[y][x] = true;
        if(adjacentMines[y][x] == 0) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int newY = y + i;
                    int newX = x + j;
                    if (newY >= 0 && newY < ROWS && newX >= 0 && newX < COLS &&!revealed[newY][newX]) {
                        revealTile(newY, newX);
                    }
                }
            }
        }
    }
}

// Function to generate the minefield
void generateMinefield() {
    memset(board, 0, sizeof(board));
    memset(revealed, 0, sizeof(revealed));
    memset(adjacentMines, 0, sizeof(adjacentMines));
    for (int i = 0; i < MINES; ++i) {
        int y, x;
        do {
            y = rand() % ROWS;
            x = rand() % COLS;
        } while (board[y][x]);
        board[y][x] = true;
    }
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    int newY = i + k;
                    int newX = j + l;
                    if (newY >= 0 && newY < ROWS && newX >= 0 && newX < COLS) {
                        adjacentMines[i][j] += board[newY][newX];
                    }
                }
            }
        }
    }
}

bool allTilesRevealed() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (!board[i][j] && !revealed[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    if (!initSDL()) {
        printf("Failed to initialize!\n");
        return -1;
    }

    generateMinefield();

    bool gameOver = false;
    while (!gameOver) {
        handleEvents();
        render();
        if (allTilesRevealed()) {
            printf("You won!\n");
            gameOver = true;
        }
    }

    closeSDL();
    return 0;
}
