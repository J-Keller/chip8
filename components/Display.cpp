//
// Created by viking on 11/21/22.
//

#include <iostream>
#include "Display.h"


void Display::clearScreen() {
    for(unsigned char y = 0; y < height; y++) {
        for (unsigned char x = 0; x < width; x++) {
            display[y][x] = 0b0;
        }
    }
}

// TODO: use actual graphics like SDL or actual console graphics
void Display::printScreen() {
    for(unsigned char y = 0; y < height; y++) {
        for (unsigned char x = 0; x < width; x++) {
            std::cout << (display[y][x] ? "#" : ".");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
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
}
