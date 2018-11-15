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

void call_editor (ARG *a) {
    //
    // CTRL + S: Save the text
    //
    if (key_ctrl && a->key == CTRL_KEY_S) {
        char *FileName = app_EditorGetFileName (ed);
        if (FileName[0]) { // CTRL + S
            FILE *fp;
            char *s = app_EditorGetText (ed);
            if ((fp = fopen (FileName, "w")) != NULL) {
                while (*s) {
                    fputc (*s, fp);
                    s++;
                }
                fclose (fp);
                printf ("\nSaved: '%s'\n", FileName);
            }
        }
    }
}

void CreateInterface (void) {

    if (text && FileName) {
        ed = app_NewEditor (NULL, ID_EDITOR1, 0, 0, text, 50000);
        app_EditorSetFileName (ed, FileName);
    } else {
        ed = app_NewEditor (NULL, ID_EDITOR1, 0, 0, "Editor PRIMEIRO\n", 50000);
    }

    app_SetSize (ed, screen->w-100, screen->h);
    app_SetFocus (ed);
    app_SetCall (ed, call_editor);
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
    printf ("Exiting With Sucess: %d\n", MRGB(255,128,0));
    return 0;
}

