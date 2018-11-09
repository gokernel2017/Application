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

int main (int argc, char **argv) {
    if (app_Init(argc,argv)) {
        app_Run (0);
    }
    return 0;
}

