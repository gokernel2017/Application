//-------------------------------------------------------------------
//
// OBJECT Editor Implementation:
//
//-------------------------------------------------------------------
//
#include "app.h"

#define EDITOR_FILE_NAME_SIZE   255
#define LINE_DISTANCE           17

typedef struct {
    char  *text; // use malloc (size)
    char  FileName [EDITOR_FILE_NAME_SIZE];
    int   pos;      // position in text: text [ pos ];
    int   col;      // x cursor position
    int   line;
    int   line_top; // first line displayed
    int   line_pos;
    int   line_count;
    //
    int   len;      // string len
    int   size;     // memory size text alloc
    int   color;    // text color
    int   bg;       // bg color
}DATA_EDITOR;

DATA_EDITOR *data;
SDL_Rect r;

static void DrawLineNumber (OBJECT *o) {
//    char buf[20];
    SDL_FillRect (screen, &(SR){ r.x+1, r.y+1, 61, r.h-2}, MRGB(240,240,240)); // line number bg
/*
    y = o->rect.y + 5;
    i = 1;
    for (;;) {
        sprintf (buf, "%04d", i++);
        DrawText (screen, buf, o->rect.x+4, y, COLOR_ORANGE);
        y += LINE_DISTANCE;
        if (y > o->rect.y+o->rect.h-LINE_DISTANCE) break;
    }
*/
}


int proc_editor (OBJECT *o, int msg, int value) {
    data = app_GetData (o);

    switch (msg) {

    case MSG_DRAW: {
        char *s = data->text;
        char buf[20];
        int line_top = 0, i = 1;
        int pos_x, pos_y;

        app_GetRect (o, &r);
        SDL_FillRect (screen, &r, data->bg); // bg
        DrawRect (screen, r.x, r.y, r.w, r.h, COLOR_ORANGE); // border
        DrawLineNumber (o);

        pos_x = r.x + 70;
        pos_y = r.y + 5;
        data->line_count = 0;
        //-------------------------------
        // Get the FIRST char DISPLAYED
        // ed->line_count,
        //-------------------------------
        while (*s) {
            if (line_top == data->line_top) break;
            if (*s == '\n') { // <-- new line
                if (line_top != data->line_top) line_top++;
                data->line_count++;
            }
            s++;
        }

        while (*s) {
            if (pos_y > (r.y + r.h)-LINE_DISTANCE)
          break;

            // area of editor
            if (pos_x < r.x+r.w-8) {
                DrawChar(screen, *s, pos_x, pos_y, data->color);
            }
            pos_x += 8;

            if (*s == '\n') { // <-- New line
                sprintf (buf, "%04d", data->line_top + i); i++;
                DrawText (screen, buf, r.x+4, pos_y, COLOR_ORANGE);
printf ("line:( %s )\n", buf);

                data->line_count++;
                pos_x = r.x + 70;// (ox + 5) - data->scroll*8;
                pos_y += LINE_DISTANCE;
            }

            s++;
        }
        // Get "ed->line_count": continue incremeting "count_ch" at END(str)
        while (*s) {
            if (*s == '\n') data->line_count++;
            s++;
        }

        //AS_rect ( ASbuf,  (ox+4+data->col*8) -data->scroll*8, 4+oy+data->line_pos*15, (ox+13+data->col*8) -data->scroll*8, 5+oy+data->line_pos*15+15, data->fg);
        DrawRect (screen,  (r.x+69+data->col*8), r.y+4+data->line_pos*15, 9, 14, COLOR_WHITE);
/*
        if (sel) {
            // Cursor ...
            AS_vline ( ASbuf, (ox+4+data->col*8) -data->scroll*8, oy+1, oy+O->h-1, data->sintax);
            AS_rect ( ASbuf,  (ox+4+data->col*8) -data->scroll*8, 4+oy+data->line_pos*15, (ox+13+data->col*8) -data->scroll*8, 5+oy+data->line_pos*15+15, data->fg);
        }
*/
        } break; // case MSG_DRAW:

    case MSG_FOCUS:
        return 1; // object focused ok

    case MSG_KEY: {
        printf ("KEY(%c) = TEXT(%s)\n", value, data->text);
        } break; // case MSG_KEY:

    }// switch (msg)

    return 0;
}

OBJECT * app_NewEditor (OBJECT *parent, int id, int x, int y, char *text, int size) {
    OBJECT *o;
    DATA_EDITOR *data;

    if ((data = (DATA_EDITOR*)malloc(sizeof(DATA_EDITOR))) == NULL)
  return NULL;

    data->text = (char*) malloc (size);
    if (text)
        sprintf (data->text, "%s", text);
    else
        data->text[0] = 0;

    data->FileName[0] = 0;
    data->pos = 0;
    data->col = 0;
    data->line = 0;
    data->line_top = 0;
    data->len = 0;
    data->size = size; // memory size text alloc
    data->color = COLOR_ORANGE;
    data->bg = 0;

    o = app_ObjectNew (proc_editor, x, y, 320, 240, id, OBJECT_TYPE_EDITOR, data);

    app_ObjectAdd (parent, o);

    return o;
}

