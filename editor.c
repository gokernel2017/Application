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

#define ID_BUTTON1    1000  // FuncList
#define ID_BUTTON2    1001  // Templat
#define ID_EDIT       1002
#define ID_EDITOR     1003
#define ID_MENU       1004

OBJECT *button1, *button2, *edit, *editor;
MENU *menu;
char *text = NULL;
char *FileName = NULL;
int find_pos;
int value = 1;

void call_button1 (ARG *a) {
    if (menu) {
        int ret;
        app_EditorListFunction (editor, menu);
        if ((ret = app_Menu (menu, 3, 33)) != -1) {
            MENU_ITEN *iten;
            if ((iten = app_MenuItenGet (menu, ret)) != NULL) {
                app_EditorFindString (editor, iten->text, 0);
            }
        }
        app_ObjectUpdate (editor);
    }
}
void call_button2 (ARG *a) {
    app_ShowDialog ("Template Not Implemented ...", DIALOG_OK);
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

    button1 = app_NewButton (NULL, ID_BUTTON1, 3, 3, "FuncList");
    button2 = app_NewButton (NULL, ID_BUTTON1, 106, 3, "Template");
    edit = app_NewEdit (NULL, ID_EDIT, 209, 3, "Find Text", 255);

    if (text && FileName) {
        editor = app_NewEditor (NULL, ID_EDITOR, 3, 33, text, 10000);
        app_EditorSetFileName (editor, FileName);
    } else {
        editor = app_NewEditor (NULL, ID_EDITOR, 3, 33, "Editor PRIMEIRO\n", 10000);
    }
    app_SetSize (editor, screen->w-100, screen->h-35);
    app_SetFocus (editor);
    app_SetCall (editor, call_editor);

    app_SetCall (button1, call_button1);
    app_SetCall (button2, call_button2);
    app_SetCall (edit, call_edit);

    menu = app_MenuCreate (400, 500);
}

void Finalize (void) {
    if (text) free (text);
    if (menu) {
        app_MenuItenClear (menu);
        free (menu);
    }
}

int main (int argc, char **argv) {
    if (app_Init(argc,argv)) {

        if (argc >= 2 && (text = app_FileOpen(argv[1])) != NULL) { 
            FileName = argv[1];
        }
        CreateInterface ();
        app_Run (NULL);
        Finalize();
    }
    printf ("Exiting With Sucess: %d\n", MRGB(128,128,128));
    return 0;
}

