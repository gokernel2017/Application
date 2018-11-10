//-------------------------------------------------------------------
//
// Basic Example:
//
// FILE:
//   basic.c
//
// COMPILE:
//   gcc basic.c -o basic libapp.a -lSDL -Wall
//
//-------------------------------------------------------------------
//
#include "src/app.h"

int count;

void loop (void) {

    printf ("IDLE: %d\n", count++);

    SDL_Delay (10);
}

int main (int argc, char **argv) {
    if (app_Init(argc,argv)) {
        app_Run (loop);
    }
    return 0;
}

