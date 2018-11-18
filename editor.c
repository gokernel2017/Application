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

#ifdef WIN32
    #include "io.h"
#else
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>
#endif

#define ID_BUTTON1    1000  // FuncList
#define ID_BUTTON2    1001  // Templat
#define ID_EDIT       1002
#define ID_EDITOR     1003
#define ID_MENU       1004

#ifdef WIN32
    #define EDITOR_TEMPLATE_DIR  "c:\\editor\\template\\"
#endif
#ifdef __linux__
    #define EDITOR_TEMPLATE_DIR  "/usr/editor/template/"
#endif

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
//        if ((ret = app_Menu (menu, 3, 33)) != -1) {
        if ((ret = app_Menu (menu, 3+50, 33)) != -1) {
            MENU_ITEN *iten;
            if ((iten = app_MenuItenGet (menu, ret)) != NULL) {
                app_EditorFindString (editor, iten->text, 0);
            }
        }
        app_ObjectUpdate (editor);
    }
}
void call_button2 (ARG *a) {
    app_ShowDialog (EDITOR_TEMPLATE_DIR, DIALOG_OK);
/*
    char buf[255];
    int ret;

    if (!menu) return;
    app_MenuItenClear (menu);

#ifdef __WIN32__
  {
  int done;
  struct _finddata_t find;
 
    sprintf (buf, "%s%s", EDITOR_TEMPLATE_DIR, "*.*");

    // NOW Find only FILEs
    done = _findfirst (buf, &find);
    do {
        if (!(find.attrib & _A_SUBDIR)) {
            sprintf (buf, "%s%s", EDITOR_TEMPLATE_DIR, find.name);
            app_MenuItenAdd (menu, buf);
        }
    } while ( !_findnext(done, &find) );
    _findclose(done);
    
  }
#endif // __WIN32__


#ifdef __linux__
  {
  DIR *dir;
  struct dirent *entry;
  struct stat s;

    sprintf (buf, "%s", EDITOR_TEMPLATE_DIR);

    //-------------------------------------------
    // Find ONLY FILES
    //-------------------------------------------
    dir = opendir(buf);
    for (;;) {
        entry = readdir(dir);
        if (!entry) break;

        if ( !(stat(entry->d_name, &s) == 0 && S_ISDIR(s.st_mode)) ) {
//            printf ("FILE: %s\n", entry->d_name);
            sprintf (buf, "%s%s", EDITOR_TEMPLATE_DIR, entry->d_name);
            app_MenuItenAdd (menu, buf);

        }
    }// for (;;)
    closedir(dir);

  }
#endif // __linux__

    if ((ret = app_Menu (menu, 106, 33)) != -1) {
        MENU_ITEN *iten;
        FILE *fp;
        int c;
        if ((iten = app_MenuItenGet (menu, ret)) != NULL) {
            if ((fp = fopen (iten->text, "r")) != NULL) {
                while ((c = fgetc(fp)) != EOF) {
printf ("%c", c);
                }
                fclose(fp);
            }
        }
    }

    app_ObjectUpdate (editor);
*/
}// call_button2 ()

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
        editor = app_NewEditor (NULL, ID_EDITOR, 3, 33, text, 50000);
        app_EditorSetFileName (editor, FileName);
    } else {
        editor = app_NewEditor (NULL, ID_EDITOR, 3, 33, "Editor PRIMEIRO\n", 50000);
    }
    app_SetSize (editor, screen->w-100, screen->h-35);
    app_SetFocus (editor);
    app_SetCall (editor, call_editor);

    app_SetCall (button1, call_button1);
    app_SetCall (button2, call_button2);
    app_SetCall (edit, call_edit);

//    menu = app_MenuCreate (400, 500);
    menu = app_MenuCreate (400, 250);
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

