//-------------------------------------------------------------------
//
// THANKS TO:
// ----------------------------------------------
//
//   01 : God the creator of the heavens and the earth in the name of Jesus Christ.
//
// ----------------------------------------------
//
// THIS FILE IS PART OF APPLICATION API:
//
// The Main Header:
//
// START DATE: 04/11/2018 - 07:00
//
//-------------------------------------------------------------------
//
#ifndef _APP_H_
#define _APP_H_

#include <SDL/SDL.h>
#ifdef WIN32
    #undef main
#endif

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------
//---------------  DEFINE / ENUM  ---------------
//-----------------------------------------------
//
#define LIBIMPORT       extern
#define BMP             SDL_Surface
//
#define COLOR_ORANGE    64512
#define CONSOLE_BG      8 // blue
#define SR              struct SDL_Rect
#define MRGB(r,g,b)     SDL_MapRGB(screen->format, r, g, b)
#define COLOR_BORDER    11184810
#define COLOR_PURPURE   12402852
#define COLOR_WHITE     16777215
#define COLOR_BLUE2     33237
#define COLOR_OBG       15724527  // object bg

// CTRL + KEY:
#define CTRL_KEY_C      3   // CTRL + C
#define CTRL_KEY_S      19  // CTRL + S
#define CTRL_KEY_V      22  // CTRL + V

//
// object send message
#define MSG_DRAW        1
#define MSG_KEY         2
#define MSG_CHAR        3   // fucused object editable
#define MSG_FOCUS       4   // set focus
#define MSG_MOUSE_DOWN  5   // click on enter
#define MSG_MOUSE_UP    6   // click on leave
#define MSG_ENTER       7   // on mouse enter
#define MSG_LEAVE       8   // on mouse leave
// "proc_object" return:
#define RET_REDRAW_ALL  10  // redraw all objects
#define RET_REDRAW      11  // redraw the current object
#define RET_CALL        12

#define DIALOG_OK       1

enum {
    OBJECT_TYPE_OBJECT = 1,
    OBJECT_TYPE_BUTTON,
    OBJECT_TYPE_EDIT,
    OBJECT_TYPE_EDITOR,
    OBJECT_TYPE_MENU
};

//-----------------------------------------------
//-------------------  STRUCT  ------------------
//-----------------------------------------------
//
typedef struct OBJECT     OBJECT; // opaque struct in file: "sgui.c"
typedef struct ARG        ARG;    // OBJECT Function Callback Argument
typedef struct TRect      TRect;
typedef struct MENU       MENU;
typedef struct MENU_ITEN  MENU_ITEN;

struct ARG { // OBJECT Function Callback Argument
    int   msg;
    int   id;
    int   x;
    int   y;
    int   key;
};
struct TRect {
    short   x,  y;
    short   w,  h;
};
struct MENU {
    int   w;
    int   h;
    int   index;
    int   top;
    int   pos_y;
    int   count;
    int   button_h; // button h
    MENU_ITEN *iten_first;
};
struct MENU_ITEN {
    char  *text;
    MENU_ITEN *next;
};

//-----------------------------------------------
//-----------------  VARIABLES  -----------------
//-----------------------------------------------
//
LIBIMPORT SDL_Surface *screen;
LIBIMPORT int key_ctrl;
LIBIMPORT int key_shift;

//-----------------------------------------------
//-----------------  PUBLIC API  ----------------
//-----------------------------------------------
//
// sgui.c | The Main Core:
//
LIBIMPORT int       app_Init          (int argc, char **argv);
LIBIMPORT void      app_Run           (void (*call) (void));
LIBIMPORT void    * app_GetData       (OBJECT *o);
LIBIMPORT void      app_GetRect       (OBJECT *o, SDL_Rect *rect);
LIBIMPORT int       app_GetType       (OBJECT *o);
LIBIMPORT OBJECT  * app_GetByID       (int id);
LIBIMPORT char    * app_FileOpen      (const char *FileName);
LIBIMPORT void      app_SetFocus      (OBJECT *o);
LIBIMPORT void      app_SetSize       (OBJECT *o, int w, int h);
LIBIMPORT void      app_SetVisible    (OBJECT *o, int visible);
LIBIMPORT int       app_Focused       (OBJECT *o);
LIBIMPORT void      app_SetCall       (OBJECT *o, void (*call) (ARG *arg));
LIBIMPORT void      app_ObjectAdd     (OBJECT *o, OBJECT *sub);
LIBIMPORT void      app_ObjectUpdate  (OBJECT *o); // draw and display
LIBIMPORT int       app_ShowDialog    (char *text, int ok);
//
LIBIMPORT OBJECT * app_ObjectNew (
    int   (*proc) (OBJECT *o, int msg, int value),
    int   x, int y, int w, int h,
    int   id,
    char  type,
    void  *data
    );

LIBIMPORT OBJECT * app_NewButton  (OBJECT *parent, int id, int x, int y, char *text);
LIBIMPORT OBJECT * app_NewEdit    (OBJECT *parent, int id, int x, int y, char *text, int size);
LIBIMPORT OBJECT * app_NewEditor  (OBJECT *parent, int id, int x, int y, char *text, int size);
LIBIMPORT OBJECT * app_NewMenu    (OBJECT *parent, int id, int x, int y);

// Editor Functions:
//
LIBIMPORT void    app_EditorSetFileName (OBJECT *o, char *FileName);
LIBIMPORT int     app_EditorFindString (OBJECT *o, char *str, int start);
LIBIMPORT char  * app_EditorGetFileName (OBJECT *o);
LIBIMPORT char  * app_EditorGetText (OBJECT *o);
LIBIMPORT void    app_EditorListFunction (OBJECT *o, MENU *menu);
LIBIMPORT void    app_EditorFree (OBJECT *o);

// Edit Functions:
LIBIMPORT char  * app_EditGetText (OBJECT *o);

//
// menu.c
//
LIBIMPORT MENU  * app_MenuCreate (int w, int h);
LIBIMPORT void    app_MenuItenAdd (MENU *m, char *text);
LIBIMPORT void    app_MenuItenClear (MENU *m);
LIBIMPORT int     app_Menu (MENU *m, int x, int y);
LIBIMPORT MENU_ITEN * app_MenuItenGet (MENU *m, int index);

//
// draw.c | Drawing Primitive:
//
LIBIMPORT void  DrawPixel   (BMP *bmp, int x, int y, int color);
LIBIMPORT void  DrawHline   (BMP *bmp, int x1, int y, int x2, int color);
LIBIMPORT void  DrawVline   (BMP *bmp, int x, int y1, int y2, int color);
LIBIMPORT void  DrawRect    (BMP *bmp, int x, int y, int w, int h, int color);
LIBIMPORT void  DrawRectR   (BMP *bmp, int x, int y, int w, int h, int color);
LIBIMPORT void  DrawChar    (BMP *bmp, char ch, int x, int y, int color);
LIBIMPORT void  DrawText    (BMP *bmp, char *text, int x, int y, int color);
LIBIMPORT void  DrawWindow  (SDL_Rect *rect);

#ifdef __cplusplus
}
#endif
#endif // ! _APP_H_

