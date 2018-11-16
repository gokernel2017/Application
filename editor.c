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

#define ID_BUTTON     1000
#define ID_EDIT       1001
#define ID_EDITOR     1002

OBJECT *button, *edit, *editor;
char *text = NULL;
char *FileName = NULL;
int find_pos;

void call_button (ARG *a) {
    app_ShowDialog ("Menu Not Implemented ...", 1);
}

//
// Line edit: CallBack
//
void call_edit (ARG *a) {
    if (a->key == SDLK_RETURN) {
        find_pos = app_EditorFindString (editor, app_EditGetText(edit), find_pos);
        if (find_pos != -1) {
            find_pos++;
            app_ObjectUpdate (editor);
        }
        else find_pos = 0;
    }
    else
    if (a->key == SDLK_TAB) {
        app_SetFocus (editor);
        app_ObjectUpdate (editor);
    }
}

//
// Editor Mult Line: CallBack
//
void call_editor (ARG *a) {
    //
    // CTRL + S: Save the text
    //
    if (key_ctrl && a->key == CTRL_KEY_S) {
        char *FileName = app_EditorGetFileName (editor);
        if (FileName[0]) { // CTRL + S
            FILE *fp;
            char *s = app_EditorGetText (editor);
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

    button = app_NewButton (NULL, ID_BUTTON, 3, 3, "Menu");
    edit = app_NewEdit (NULL, ID_EDIT, 106, 3, "Find Text", 255);

    if (text && FileName) {
        editor = app_NewEditor (NULL, ID_EDITOR, 3, 33, text, 50000);
        app_EditorSetFileName (editor, FileName);
    } else {
        editor = app_NewEditor (NULL, ID_EDITOR, 3, 33, "Editor PRIMEIRO\n", 50000);
    }
    app_SetSize (editor, screen->w-100, screen->h-35);
    app_SetFocus (editor);
    app_SetCall (editor, call_editor);

    app_SetCall (button, call_button);
    app_SetCall (edit, call_edit);
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
    printf ("Exiting With Sucess: %d\n", MRGB(128,128,128));
    return 0;
}

