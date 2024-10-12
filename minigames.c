#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640       // Width of the game window
#define SCREEN_HEIGHT 480      // Height of the game window
#define TILE_SIZE 40           // Size of each tile in the grid
#define ROWS 10                // Number of rows in the grid
#define COLS 10                // Number of columns in the grid
#define MINES 10               // Total number of mines

// Global variables for the SDL window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Game-related arrays
bool board[ROWS][COLS];            // Stores whether a tile contains a mine (true if mine is present)
bool revealed[ROWS][COLS];         // Tracks whether a tile is revealed
int adjacentMines[ROWS][COLS];      // Stores the number of adjacent mines for each tile

// Function declarations
void revealTile(int y, int x);

// Initialize SDL and create the game window and renderer
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {  // Initialize SDL Video subsystem
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create a window with specified size and position
    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create a renderer for the window with hardware acceleration
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;  // SDL initialized successfully
}

// Close SDL and clean up resources
void closeSDL() {
    SDL_DestroyRenderer(renderer);  // Destroy the renderer
    SDL_DestroyWindow(window);      // Destroy the window
    SDL_Quit();                     // Quit SDL subsystems
}

// Render the game board
void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Set background color to white
    SDL_RenderClear(renderer);                             // Clear the screen

    SDL_Rect tileRect;                                     // Rectangle to represent each tile
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    // Loop through each tile in the grid
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            tileRect.x = j * TILE_SIZE;                    // Set the tile's position
            tileRect.y = i * TILE_SIZE;

            if (revealed[i][j]) {                          // If the tile is revealed
                SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);  // Light gray for revealed tile
                SDL_RenderFillRect(renderer, &tileRect);                // Fill the tile

                if (board[i][j]) {                                     // If the tile contains a mine
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for mines
                    SDL_RenderFillRect(renderer, &tileRect);           // Render the mine
                } else {
                    char buffer[10];
                    sprintf(buffer, "%d", adjacentMines[i][j]);        // Display number of adjacent mines
                    SDL_Surface* surface = IMG_Load("number.png");     // Placeholder image (load your image)
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect textRect = {tileRect.x + 10, tileRect.y + 10, 20, 20};
                    SDL_RenderCopy(renderer, texture, NULL, &textRect); // Render the number on the tile
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
            } else {                                                   // If the tile is not revealed
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);   // Dark gray for unrevealed tiles
                SDL_RenderDrawRect(renderer, &tileRect);                // Draw the tile's outline
            }
        }
    }

    SDL_RenderPresent(renderer);   // Update the screen
}

// Handle player input events
void handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {  // Poll for events
        if (e.type == SDL_QUIT) {     // Close window event
            closeSDL();               // Clean up SDL resources
            exit(0);                  // Exit the program
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {  // Mouse click event
            int x = e.button.x / TILE_SIZE;          // Calculate tile X coordinate from mouse position
            int y = e.button.y / TILE_SIZE;          // Calculate tile Y coordinate

            if (e.button.button == SDL_BUTTON_LEFT) {  // Left-click to reveal tile
                revealTile(y, x);                      // Reveal the clicked tile
            } else if (e.button.button == SDL_BUTTON_RIGHT) {  // Right-click to flag tile
                revealed[y][x] = !revealed[y][x];              // Toggle flag on/off
            }
        }
    }
}

// Reveal a tile on the board
void revealTile(int y, int x) {
    if (board[y][x]) {           // If the tile contains a mine
        printf("Game over!\n");  // Print "Game over" message
        closeSDL();              // Close SDL
        exit(0);                 // End the program
    } else {
        revealed[y][x] = true;   // Mark the tile as revealed
        if (adjacentMines[y][x] == 0) {  // If the tile has no adjacent mines
            // Recursively reveal surrounding tiles
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int newY = y + i;
                    int newX = x + j;
                    if (newY >= 0 && newY < ROWS && newX >= 0 && newX < COLS && !revealed[newY][newX]) {
                        revealTile(newY, newX);  // Reveal neighboring tiles recursively
                    }
                }
            }
        }
    }
}

// Generate the minefield and calculate adjacent mine counts
void generateMinefield() {
    memset(board, 0, sizeof(board));         // Clear the board array
    memset(revealed, 0, sizeof(revealed));   // Clear the revealed array
    memset(adjacentMines, 0, sizeof(adjacentMines));  // Clear the adjacent mine count array

    // Place mines randomly on the board
    for (int i = 0; i < MINES; ++i) {
        int y, x;
        do {
            y = rand() % ROWS;  // Randomly select a row
            x = rand() % COLS;  // Randomly select a column
        } while (board[y][x]);  // Ensure that the tile is empty before placing a mine
        board[y][x] = true;     // Place the mine
    }

    // Calculate the number of adjacent mines for each tile
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    int newY = i + k;
                    int newX = j + l;
                    if (newY >= 0 && newY < ROWS && newX >= 0 && newX < COLS) {
                        adjacentMines[i][j] += board[newY][newX];  // Count neighboring mines
                    }
                }
            }
        }
    }
}

// Check if all non-mine tiles are revealed
bool allTilesRevealed() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (!board[i][j] && !revealed[i][j]) {
                return false;  // If any non-mine tile is still hidden, return false
            }
        }
    }
    return true;  // All non-mine tiles are revealed
}

int main() {
    if (!initSDL()) {   // Initialize SDL and check for errors
        printf("Failed to initialize!\n");
        return -1;      // Exit program if SDL initialization fails
    }

    generateMinefield();  // Generate the minefield

    bool gameOver = false;  // Flag for game status
    while (!gameOver) {     // Main game loop
        handleEvents();     // Handle player input
        render();           // Render the game board

        // Check if all non-mine tiles are revealed
        if (allTilesRevealed()) {
            printf("You won!\n");  // Print "You won" message
            gameOver = true;       // End the game loop
        }
    }

    closeSDL();  // Clean up SDL resources and close the game

    return 0;  // Exit the program
}
