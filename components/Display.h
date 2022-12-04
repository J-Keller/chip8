//
// Created by viking on 11/21/22.
//

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H


#include <SDL.h>
#include <ncurses.h>


class Display {
public:
    Display();
    ~Display();

    void clearScreen();
    void printScreenSDL();
    void printDebugInfo(unsigned char* registers, unsigned char* memory, unsigned short pc);
    bool getPixel(unsigned char, unsigned char);
    void setPixel(unsigned char, unsigned char, bool);
    bool getCollisionRight(unsigned char);
    bool getCollisionBottom(unsigned char);
protected:
private:

    void initDebugStuff();

    // Debug Stuff
    WINDOW *registerWindow;
    WINDOW *memoryWindow;
    WINDOW *pcWindow;

    // SDL Stuff
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Color grid_background;
    SDL_Color grid_line_color;
    SDL_Color grid_pixel_color;

    // Basic Stuff
    static const unsigned char width = 64;
    static const unsigned char height = 32;
    static const unsigned char gridSize = 10;
    // 0 -> black; 1 -> color (TODO: color setting)
    bool display[height][width] = {};
};


#endif //CHIP8_DISPLAY_H
