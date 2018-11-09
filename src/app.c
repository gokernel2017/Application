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
// The Main Core:
//
// START DATE: 04/11/2018 - 07:00
//
//-------------------------------------------------------------------
//
#include "app.h"

#define DIALOG_TEXT_SIZE  100
#define ID_YES            1
#define ID_NO             0

// size: 64
// size: 52
struct OBJECT { // opaque struct
    int       (*proc) (OBJECT *o, int msg, int value);
    void      (*call) (ARG *arg); // user function callback
    short     x;
    short     y;
    SDL_Rect  rect; //  real position from the gui ... this is computed from parent
    int       id;
    char      type;
    char      focus;
    char      visible;
    OBJECT    *parent;
    OBJECT    *first;
    OBJECT    *next;
    void      *data; // any information about object
};
typedef struct {
    char  text [DIALOG_TEXT_SIZE];
    int   fg;
    int   bg;
}DATA_DIALOG;

static void draw_bg (void);
static void app_ObjectMouseFind (OBJECT *o);
static void app_ObjectDrawAll (OBJECT *o);
static void app_UpdatePos (OBJECT *obj);

//-----------------------------------------------
//-----------------  VARIABLES  -----------------
//-----------------------------------------------
//
SDL_Surface *screen;

DATA_DIALOG dialog_data;

// Dialog:
//-----------------------------------------------
static OBJECT * dialog_root = NULL;
static OBJECT * dialog      = NULL;
static OBJECT * dlgYES      = NULL;
static OBJECT * dlgNO       = NULL;
//-----------------------------------------------
static OBJECT * root         = NULL; // the root object
static OBJECT * current      = NULL; // the current object
static OBJECT * mouse_find   = NULL; // object on mouse
static OBJECT * object_focus = NULL; // focused object
static OBJECT * object_click = NULL;
//-----------------------------------------------

//-----------------------------------------------
// OBJECT Callback Function Argument
//
//   void (*call) (ARG *a);
//
//   OBJECT->call (&arg);
//-----------------------------------------------
//
static ARG  arg;

static int
    state,
    quit,
    dialog_quit, dialog_ret,
    count,
    key,
    mx, my // mouse_x, mouse_y
    ;

static int proc_null (OBJECT *o, int msg, int i) {
    return 0;
}

int app_Init (int argc, char **argv) {
    static int init = 0;
    int w = 0, h = 0, bpp = 16, flags = SDL_HWSURFACE, i;

    if (init) return 1;
    init = 1;
    SDL_Init (SDL_INIT_VIDEO);
    SDL_WM_SetCaption ("Application API:", NULL);
    #ifdef _WIN32
    SDL_putenv ("SDL_VIDEO_CENTERED=center");
    #endif

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-w") && argc > i)
            w = atoi(argv[i+1]);
        if (!strcmp(argv[i], "-h") && argc > i)
            h = atoi(argv[i+1]);
        if (!strcmp(argv[i], "-noframe"))
           flags |= SDL_NOFRAME;

    }
    if (w <= 0) w = 800;
    if (h <= 0) h = 600;

    screen = SDL_SetVideoMode (w, h, bpp, flags);

    SDL_EnableUNICODE (1);
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); // For keypressed

    root = app_ObjectNew (proc_null,0,0,0,0,0,0,NULL);

    atexit (SDL_Quit);

    return 1;
}

void app_UpdateGui (OBJECT *o) {
    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
    switch (ev.type) {
    case SDL_MOUSEMOTION:
        mx = ev.motion.x;
        my = ev.motion.y;
        arg.x = mx;
        arg.y = my;

        //---------------------------------------
        // set: mouse_find
        //---------------------------------------
        mouse_find = NULL;
        app_ObjectMouseFind (o); // <<<<<<< set mouse_find >>>>>>>

        //-----------------------------------------------
        //-----------  MSG_ENTER / MSG_LEAVE  -----------
        //-----------------------------------------------
        //
        // set object 'current':
        //
        //-----------------------------------------------
        if (mouse_find != current) {
            int ret;

            // MSG_ENTER
            if (mouse_find && (ret = mouse_find->proc (mouse_find,MSG_ENTER,0))) {
                // redraw and update
                mouse_find->proc (mouse_find, MSG_DRAW, 1);
                SDL_UpdateRect (screen, mouse_find->rect.x, mouse_find->rect.y, mouse_find->rect.w, mouse_find->rect.h);
                // send callback : MSG_ENTER
                if (ret == RET_CALL && mouse_find->call) {
                    arg.msg = MSG_ENTER;
                    mouse_find->call (&arg);
                }
            }
            // MSG_LEAVE
            if (current && (ret = current->proc (current,MSG_LEAVE,0))) {
                // redraw and update
                current->proc (current, MSG_DRAW, 0);
                SDL_UpdateRect (screen, current->rect.x, current->rect.y, current->rect.w, current->rect.h);
                // send callback : MSG_LEAVE
                if (ret == RET_CALL && current->call) {
                    arg.msg = MSG_LEAVE;
                    current->call (&arg);
                }
            }

            current = mouse_find;

        }// if (mouse_find != current)

        break;//: case SDL_MOUSEMOTION:

    //-----------------------------------------------
    // On Mouse Click ENTER: MSG_FOCUS
    //-----------------------------------------------
    case SDL_MOUSEBUTTONDOWN:
        mx = ev.button.x;
        my = ev.button.y;

        if (current) {

            //
            // set object clicked ... to use in event: SDL_MOUSEBUTTONUP.
            //
            object_click = current;

            if (current != object_focus && current->proc (current, MSG_FOCUS, 0)) {
                // draw the old
                if (object_focus) {
                    object_focus->focus = 0;
                    object_focus->proc (object_focus, MSG_DRAW, 0);
                    SDL_UpdateRect (screen, object_focus->rect.x, object_focus->rect.y, object_focus->rect.w, object_focus->rect.h);
                }
                current->focus = 1;
                // now set object_focus:
                object_focus = current;
                object_focus->proc (object_focus, MSG_DRAW, 1);
                SDL_UpdateRect (screen, object_focus->rect.x, object_focus->rect.y, object_focus->rect.w, object_focus->rect.h);
            }
            if (current->proc (current, MSG_MOUSE_DOWN, 0) == RET_CALL && current->call) {
                arg.msg = MSG_MOUSE_DOWN;
                arg.id = current->id;
                current->call (&arg);
            }

        }// if (current)

        break;// case SDL_MOUSEBUTTONDOWN:

    case SDL_MOUSEBUTTONUP:

        if (object_click && object_click == current) {
            if (object_click->proc (object_click, MSG_MOUSE_UP, 0) == RET_CALL && object_click->call) {
                arg.msg = MSG_MOUSE_UP;
                arg.id = object_click->id;
                object_click->call (&arg);
            }
        }
        object_click = NULL;

        break;// case SDL_MOUSEBUTTONUP:

    case SDL_KEYDOWN:
        if ((key = ev.key.keysym.unicode)==0)
            key = ev.key.keysym.sym;

        if (key == SDLK_F12 && o == root) {
            quit = app_ShowDialog("Application API - Exit ?");
        }

        if (object_focus && object_focus->visible && object_focus->focus) {
            object_focus->proc (object_focus, MSG_KEY, key);
        }

        break; // case SDL_KEYDOWN:

    }// switch (ev.type)
    }// while (SDL_PollEvent(&ev))

    SDL_Delay (10);

    if (state==RET_REDRAW_ALL) {
        state = 0;
        if (o == root)
            draw_bg ();
        app_ObjectDrawAll (o);
        SDL_Flip (screen);
    }

}// app_UpdateGui ()

void app_Run (void (*call) (void)) {

    app_UpdatePos (root);
    state = RET_REDRAW_ALL;
    quit = 0;

    while (!quit) {

        app_UpdateGui (root);

    }
}

static void draw_bg (void) {
    int color1 = SDL_MapRGB (screen->format, 254,238,204);
//        int color2 = SDL_MapRGB (screen->format, 255,219,164);
    int hh = screen->h/3;

    SDL_FillRect (screen, &(SR){ 1, 1, screen->w-1, hh }, color1);
    SDL_FillRect (screen, &(SR){ 1, hh, screen->w-1, hh }, COLOR_BLUE2);
    SDL_FillRect (screen, &(SR){ 1, hh+hh, screen->w-1, hh }, color1);

    SDL_FillRect (screen, &(SR){ 1, hh-20, screen->w-1, 20 }, COLOR_ORANGE);
    SDL_FillRect (screen, &(SR){ 1, hh+hh, screen->w-1, 20 }, COLOR_ORANGE);

    DrawText (screen, "To Exit Press The KEY: F12", 100, (screen->h/2), COLOR_WHITE);
}

OBJECT * app_ObjectNew (
    int (*proc) (OBJECT *o, int msg, int value),
    int   x, int y, int w, int h,
    int   id,
    char  type,
    void *data
) {
    OBJECT *o;

    if ((o = (OBJECT*)malloc(sizeof(OBJECT)))==NULL)
  return NULL;
    o->proc = proc;
    o->call = NULL;
    o->x = x;
    o->y = y;
    o->rect.w = w;
    o->rect.h = h;
    o->id = id;
    o->type = type;
    o->focus = 0;
    o->visible = 1;
    o->parent = NULL;
    o->first = NULL;
    o->next = NULL;
    o->data = data;
    return o;
}

void * app_GetData (OBJECT *o) {
    return o->data;
}

void app_GetRect (OBJECT *o, SDL_Rect *rect) {
    *rect = o->rect;    
}

int app_Focused (OBJECT *o) {
    return o->focus;
}

void app_ObjectAdd (OBJECT *o, OBJECT *sub) {

    if (o==NULL) o = root;

    if (o && sub) {
        sub->parent = o;
        if (!o->first) {
            o->first = sub; // the index to first object
        } else {
            OBJECT *aux = o->first; // the index to first object

            while (aux->next != NULL)
                aux = aux->next;
            aux->next = sub;
        }
        state = RET_REDRAW_ALL;
    }
}

OBJECT * app_GetByID (int id) {
    OBJECT *sub = root->first;
    while (sub) {
        if (sub->id == id)
      return sub;
        if (sub->first) {
            app_GetByID (id);
        }
        sub = sub->next;
    }
    return NULL;
}

static void app_ObjectMouseFind (OBJECT *obj) {
    OBJECT *sub = obj->first;
    while (sub) {
        if (sub->visible && mx > sub->rect.x && mx < sub->rect.x+sub->rect.w && my > sub->rect.y && my < sub->rect.y+sub->rect.h) {
            mouse_find = sub;
            if (sub->first && sub->first->visible) {
                app_ObjectMouseFind (sub);
            }
        }
        sub = sub->next;
    }
}
static void app_ObjectDrawAll (OBJECT *obj) {
    OBJECT *sub = obj->first;
    while (sub) {
        if (sub->visible) {
            sub->proc (sub, MSG_DRAW, 0);
            if (sub->first && sub->first->visible) {
                app_ObjectDrawAll (sub);
            }
        }
        sub = sub->next;
    }
}

void app_ObjectUpdate (OBJECT *o) {
    if (o && o->visible) {
        o->proc (o,MSG_DRAW,1);
        SDL_UpdateRect (screen, o->rect.x, o->rect.y, o->rect.w, o->rect.h);
    }
}

static void app_UpdatePos (OBJECT *obj) {
    OBJECT *o = obj->first;
    while (o) {
        OBJECT *par = o;
        int x, y;
        x = par->x;
        y = par->y;
        while (par->parent) {
            par = par->parent;
            x += par->x;
            y += par->y;
        }
        //-----------------
        // update position:
        //-----------------
        o->rect.x = x;
        o->rect.y = y;
        //-----------------

        if (o->first) {
            app_UpdatePos (o);
        }

    o = o->next;
    }
}


void app_SetFocus (OBJECT *o) {
    if (o && o->visible) {
        // set old UNFOCUS
        if (object_focus)
            object_focus->focus = 0;
        o->focus = 1;
        object_focus = o;
    }
}

void app_SetCall (OBJECT *o, void (*call) (ARG *arg)) {
    if (o)
        o->call = call;
}



int proc_dialog (OBJECT *o, int msg, int value) {
    if (msg == MSG_DRAW) {
        SDL_Rect r;
        int x;
        char *s = dialog_data.text;

        app_GetRect(o, &r);
        SDL_FillRect (screen, &r, dialog_data.bg);
        DrawRectR (screen, r.x, r.y, r.w, r.h, dialog_data.fg);
        x = r.x+10;
        while (*s) {
            DrawChar (screen, *s, x, r.y+10, dialog_data.fg);
            x += 8;
            if (x > r.x+r.w) break;
            s++;
        }
        return 0;
    }
    if (msg == MSG_KEY) {
        if (value == 'Y' || value == 'y') {
            dialog_quit = 1;
            dialog_ret = 1;
        }
        if (value == 'N' || value == 'n') {
            dialog_quit = 1;
            dialog_ret = 0;
        }
    }

    return 0;
}

void call_dialog (ARG *arg) {
    dialog_quit = 1;
    dialog_ret = arg->id;
}

int app_ShowDialog (char *text) {

    if (dialog_root == NULL) {
        if ((dialog_root = app_ObjectNew (proc_null,0,0,0,0,0,0,NULL)) != NULL) {
printf ("Criando DIALOG\n");
            dialog_data.fg = COLOR_ORANGE;
            dialog_data.bg = COLOR_WHITE;
            dialog = app_ObjectNew (proc_dialog,(screen->w/2)-250,(screen->h/2)-60,500,120,0,0,&dialog_data);
            app_ObjectAdd (dialog_root, dialog);
            dlgYES = app_NewButton (dialog, ID_YES, 140, 50, "YES");
            dlgNO  = app_NewButton (dialog, ID_NO,  260, 50, "NO");
            app_SetCall (dlgYES, call_dialog);
            app_SetCall (dlgNO, call_dialog);
        }
    }

    if (dialog_root && dialog) {
        OBJECT *old_focus;

        app_UpdatePos (dialog_root);
        state = RET_REDRAW_ALL;
        dialog_ret = 0;
        dialog_quit = 0;
        dialog->visible = 1;

        if (text && strlen(text) < DIALOG_TEXT_SIZE-1) {
            sprintf (dialog_data.text, "%s", text);
        } else {
            dialog_data.text[0] = 0;
        }

        current = mouse_find = NULL;
        old_focus = object_focus; // save object focus
        app_SetFocus (dialog);

        while (!dialog_quit) {

            app_UpdateGui (dialog_root);

        }
        key = 0;

        current = mouse_find = NULL;
        object_focus = old_focus; // restore object focus
        if (object_focus)
            app_SetFocus (object_focus);
        dialog->visible = 0;
        quit = 0;

        state = RET_REDRAW_ALL;
        
    }

    return dialog_ret;
}
