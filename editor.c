//-------------------------------------------------------------------
//
// Editor Test:
//
// COMPILE:
//   gcc editor.c -o editor libapp.a -lSDL -Wall
//
// USAGE:
//   editor <FileName.txt>
//
// TO SAVE THE TEXT (in editor):
//   CTRL + S
//
//-------------------------------------------------------------------
//
#include "src/app.h"

#define ID_EDITOR1    1000
#define ID_EDITOR2    1001

OBJECT *ed;
char *text = NULL;
char *FileName = NULL;

void CreateInterface (void) {

    if (text && FileName) {
        ed = app_NewEditor (NULL, ID_EDITOR1, 0, 0, text, 50000);
        app_EditorSetFileName (ed, FileName);
    } else {
        ed = app_NewEditor (NULL, ID_EDITOR1, 0, 0, "Editor PRIMEIRO\n", 50000);
    }

    app_SetSize (ed, screen->w-100, screen->h);
    app_SetFocus (ed);
}

int main (int argc, char **argv) {
    if (app_Init(argc,argv)) {

        if (argc >= 2 && (text = app_FileOpen(argv[1])) != NULL) { 
            FileName = argv[1];
        }
        CreateInterface ();
        app_Run (NULL);

        if (text)
            free (text);
    }
    printf ("Exiting With Sucess:\n");
    return 0;
}

