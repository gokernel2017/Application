//-------------------------------------------------------------------
//
// Example Button:
//
// FILE:
//   ex_button.c
//
// COMPILE:
//   gcc ex_button.c -o ex_button libsgui.a -lSDL -Wall
//
//-------------------------------------------------------------------
//
#include "src/app.h"

#define ID_BUTTON1    1500
#define ID_BUTTON2    2000

OBJECT *bt1, *bt2;

void call_button (ARG *a) {
//    static int count = 0;
    printf ("ShowDialog = %d\n", app_ShowDialog("Select YES/NO", 0));
//    printf ("Call button ID( %d ): %d\n", a->id, count++);
}

void CreateInterface (void) {

    bt1 = app_NewButton (0, ID_BUTTON1, 100, 100, "Button 01");
    bt2 = app_NewButton (0, ID_BUTTON2, 330, 100, "Button 02");

    app_SetCall (bt1, call_button);
    app_SetCall (bt2, call_button);
}

int main (int argc, char **argv) {
    if (app_Init(argc,argv)) {
        CreateInterface ();
        app_Run (0);
        printf ("Exiting With Sucess:\n");
    }
    return 0;
}

