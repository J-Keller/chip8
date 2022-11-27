//
// Created by viking on 11/22/22.
//

#include "Input.h"
#include <SDL.h>

bool Input::isKeyDown(unsigned char key) {
    const unsigned char *state = SDL_GetKeyboardState(nullptr);

    switch(key) {
        case 0x0:
            return state[SDL_SCANCODE_C];
        case 0x1:
            return state[SDL_SCANCODE_2];
        case 0x2:
            return state[SDL_SCANCODE_3];
        case 0x3:
            return state[SDL_SCANCODE_4];
        case 0x4:
            return state[SDL_SCANCODE_W];
        case 0x5:
            return state[SDL_SCANCODE_E];
        case 0x6:
            return state[SDL_SCANCODE_R];
        case 0x7:
            return state[SDL_SCANCODE_S];
        case 0x8:
            return state[SDL_SCANCODE_D];
        case 0x9:
            return state[SDL_SCANCODE_F];
        case 0xA:
            return state[SDL_SCANCODE_X];
        case 0xB:
            return state[SDL_SCANCODE_V];
        case 0xC:
            return state[SDL_SCANCODE_5];
        case 0xD:
            return state[SDL_SCANCODE_T];
        case 0xE:
            return state[SDL_SCANCODE_G];
        case 0xF:
            return state[SDL_SCANCODE_B];
        default:
            return false;
    }
}

unsigned char Input::getKey() {
    const unsigned char *state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_C]) return 0x0;
    if (state[SDL_SCANCODE_2]) return 0x1;
    if (state[SDL_SCANCODE_3]) return 0x2;
    if (state[SDL_SCANCODE_4]) return 0x3;
    if (state[SDL_SCANCODE_W]) return 0x4;
    if (state[SDL_SCANCODE_E]) return 0x5;
    if (state[SDL_SCANCODE_R]) return 0x6;
    if (state[SDL_SCANCODE_S]) return 0x7;
    if (state[SDL_SCANCODE_D]) return 0x8;
    if (state[SDL_SCANCODE_F]) return 0x9;
    if (state[SDL_SCANCODE_X]) return 0xA;
    if (state[SDL_SCANCODE_V]) return 0xB;
    if (state[SDL_SCANCODE_5]) return 0xC;
    if (state[SDL_SCANCODE_T]) return 0xD;
    if (state[SDL_SCANCODE_G]) return 0xE;
    if (state[SDL_SCANCODE_B]) return 0xF;
    return 0x10;
}
