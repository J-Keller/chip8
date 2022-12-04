//
// Created by viking on 11/21/22.
//

#include <iostream>
#include "Display.h"


WINDOW *createNewWindow(int width, int height, int xPos, int yPos) {
    WINDOW *local_win;

    local_win = newwin(height, width, yPos, xPos);
    box(local_win, 0 , 0);		/* 0, 0 gives default characters
					             * for the vertical and horizontal
					             * lines
					             */
    wrefresh(local_win);		/* Show that box 		*/

    if(local_win == nullptr) {
        std::cerr << "window cannot be created" << std::endl;
    }

    return local_win;
}

void destroyWindow(WINDOW *window) {
    /* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners
	 * and so an ugly remnant of window.
	 */
    wborder(window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(window);
    delwin(window);
}

void Display::clearScreen() {
    for(unsigned char y = 0; y < height; y++) {
        for (unsigned char x = 0; x < width; x++) {
            display[y][x] = 0b0;
        }
    }
}

void Display::printScreenSDL() {
    SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,grid_background.b, grid_background.a);
    SDL_RenderClear(renderer);

    // Draw grid lines.
    SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,grid_line_color.b, grid_line_color.a);

    for (int x = 0; x < 1 + width * gridSize; x += gridSize) {
        SDL_RenderDrawLine(renderer, x, 0, x, height * gridSize);
    }

    for (int y = 0; y < 1 + height * gridSize; y += gridSize) {
        SDL_RenderDrawLine(renderer, 0, y, width * gridSize, y);
    }

    SDL_SetRenderDrawColor(renderer, grid_pixel_color.r, grid_pixel_color.g, grid_pixel_color.b, grid_pixel_color.a);
    for(unsigned char y = 0; y < height; y++) {
        for (unsigned char x = 0; x < width; x++) {
            if (display[y][x]) {
                SDL_Rect rect = {
                        x * gridSize,
                        y * gridSize,
                        gridSize,
                        gridSize
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Display::initDebugStuff() {
    // registers
    unsigned char registerWindowWidth = 19;
    unsigned char registerWindowHeight = 10;
    unsigned char registerWindowXPos = 1;
    unsigned char registerWindowYPos = 1;

    registerWindow = createNewWindow(registerWindowWidth, registerWindowHeight, registerWindowXPos, registerWindowYPos);

    // ram
    unsigned short memoryWindowWidth = 130;
    unsigned char memoryWindowHeight = 66;
    unsigned char memoryWindowXPos = 1;
    unsigned char memoryWindowYPos = 12;

    memoryWindow = createNewWindow(memoryWindowWidth, memoryWindowHeight, memoryWindowXPos, memoryWindowYPos);
}

void Display::printDebugInfo(unsigned char *registers, unsigned char* memory) {
    // registers
    mvwprintw(registerWindow, 0, 0, "registers");
    for (unsigned char registerNr = 0x0; registerNr < 0x8; registerNr++) {
        mvwprintw(registerWindow, registerNr + 1, 1, "%.2i: 0x%.2X", registerNr, registers[registerNr]);
    }
    for (unsigned char registerNr = 0x8; registerNr <= 0xF; registerNr++) {
        mvwprintw(registerWindow, registerNr - 7, 10, "%.2i: 0x%.2X", registerNr, registers[registerNr]);
    }
    wrefresh(registerWindow);

    // ram
    mvwprintw(memoryWindow, 0, 0, "memory");
    for(unsigned short i; i < 4096; i++) {
        mvwprintw(memoryWindow, (i / 64) + 1, (i % 64) * 2 + 1, "%.2X", memory[i]);
    }
    wrefresh(memoryWindow);

    // TODO: program code
    // TODO: program counter
}

bool Display::getPixel(unsigned char x, unsigned char y) {
    return display[y][x];
}

void Display::setPixel(unsigned char x, unsigned char y, bool state) {
    display[y][x] = state;
}

bool Display::getCollisionRight(unsigned char x) {
    return x >= width;
}

bool Display::getCollisionBottom(unsigned char y) {
    return y >= height;
}

Display::Display() {
    // init ncurses
    initscr();
    cbreak();
    refresh();

    initDebugStuff();

    // init SDL
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * gridSize, height * gridSize, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
    grid_background = {22, 22, 22, 255}; // Barely Black
    grid_line_color = {44, 44, 44, 255}; // Dark grey
    grid_pixel_color = {255, 255, 255, 255}; // White
}

Display::~Display() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // destroy debug stuff
    destroyWindow(registerWindow);
    endwin();
}
