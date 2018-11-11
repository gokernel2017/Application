//-------------------------------------------------------------------
//
// OBJECT Editor Implementation:
//
//-------------------------------------------------------------------
//
#include "app.h"

#define EDITOR_FILE_NAME_SIZE   255
#define LINE_DISTANCE           17
//#define LINE_DISTANCE           15

#define COLOR_COMMENT           63519
#define CTRL_KEY_S              19

typedef struct {
    char  *text; // use malloc (size)
    char  FileName [EDITOR_FILE_NAME_SIZE];
    int   pos;      // position in text: text [ pos ];
    int   col;      // x cursor position
    int   scroll;
    int   line;
    int   line_top; // first line displayed
    int   line_pos;
    int   line_count;
    //
    int   line_ini;
    int   line_len;
    //
    int   len;      // string len
    int   size;     // memory size text alloc
    int   saved;
    int   color;    // text color
    int   bg;       // bg color
}DATA_EDITOR;

DATA_EDITOR *data;
SDL_Rect r;

void InsertChar (char *string, register int index, int ch) {
  register int x = strlen(string);

    while (x >= index){
        string[x+1] = string[x];
        x--;
    }
    string[index] = ch;
}

void app_EditorSetFileName (OBJECT *o, char *FileName) {
    DATA_EDITOR *data = app_GetData (o);
    if (data && strlen(FileName) < EDITOR_FILE_NAME_SIZE-1) {
        sprintf (data->FileName, "%s", FileName);
    }
}

int proc_editor (OBJECT *o, int msg, int value) {
    data = app_GetData (o);
    char *s = data->text;

    app_GetRect (o, &r);

    switch (msg) {

    case MSG_DRAW: {
        char buf[50];
        int line_top = 0, i = 1;
        int pos_x = (r.x + 70) - data->scroll*8;
//        int pos_x = (r.x + 5) - data->scroll*8;
        int pos_y = r.y + 5;
        int color = data->color;
        int comment = 0; // commentary text color

        SDL_FillRect (screen, &r, data->bg); // bg
        SDL_FillRect (screen, &(SR){ r.x, r.y, 61, r.h}, MRGB(240,240,240)); // lines numbers bg
        DrawRect (screen, r.x, r.y, r.w, r.h, COLOR_ORANGE); // border

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
            // size h:
            if (pos_y > (r.y + r.h)-LINE_DISTANCE)
          break;

            if (s[0]=='/' && s[1]=='/')
                comment = 1;

            if (comment) {
                color = COLOR_COMMENT;//MRGB(100,100,100);
                if (*s=='\n')
                    comment = 0;
            } else {
                color = COLOR_ORANGE;
            }

            // area of editor
            if (pos_x < r.x+r.w-8) {
                DrawChar(screen, *s, pos_x, pos_y, color);
            }
            pos_x += 8;

            if (*s == '\n') { // <-- New line
                // draw lines numbers
                sprintf (buf, "%04d", data->line_top + i); i++;
                DrawText (screen, buf, r.x+4, pos_y, COLOR_ORANGE);

                data->line_count++;
                //pos_x = r.x + 70;// (ox + 5) - data->scroll*8;
                pos_x = (r.x + 70) - data->scroll*8;
//                pos_x = (r.x + 5) - data->scroll*8;
                pos_y += LINE_DISTANCE;
            }

            s++;
        }
        // Get "ed->line_count": continue incremeting "count_ch" at END(str)
        while (*s) {
            if (*s == '\n') data->line_count++;
            s++;
        }

        // draw cursor position
        DrawRect (screen,  (r.x+69+data->col*8) -data->scroll*8, r.y+4+data->line_pos * LINE_DISTANCE, 9, 14, COLOR_WHITE);
        DrawVline (screen, (r.x+69+data->col*8) -data->scroll*8, r.y+1, r.y+r.h-2, COLOR_WHITE);

        // display: LINE NUMBER, COL, ...
        //
        SDL_FillRect (screen, &(SR){ r.x+62, r.y+r.h-LINE_DISTANCE, r.w-62, LINE_DISTANCE}, 0); // BG: LINE: COL:
        DrawHline (screen, r.x+60, r.y+r.h-LINE_DISTANCE, r.x+r.w, COLOR_WHITE);
        sprintf (buf, "LINE: %d/%d  COL: %d - LEN/SIZE( %d / %d ) | %d = '%c' ", data->line+1, data->line_count, data->col+1, data->len, data->size, data->text[data->pos], data->text[data->pos]);
        DrawText (screen, buf, r.x+70, r.y+r.h-15, COLOR_WHITE);

        } break; // case MSG_DRAW:

    case MSG_FOCUS:
        return 1; // object focused ok

    case MSG_KEY: {
        char *str = data->text;
        int count;

        if (key_ctrl) {

            if (value == CTRL_KEY_S && data->FileName[0]) { // CTRL + S
                FILE *fp;
                char *s = data->text;
                if ((fp = fopen (data->FileName, "w")) != NULL) {
                    while (*s) {
                        fputc (*s, fp);
                        s++;
                    }
                    fclose (fp);
                    printf ("\nSaved: '%s'\n", data->FileName);
                }
            }

      return 0;
        }

        if (value == SDLK_UP && data->line > 0) {
            if (data->line_pos > 0)
                data->line_pos--;
            else
                data->line_top--;
            data->line--;
        }
        else if (value == SDLK_DOWN && data->line < data->line_count-1) {
            // if last line(DISPLAYED)
            if ((data->line_pos*LINE_DISTANCE)+34 >= r.h-14 && (data->line_pos*LINE_DISTANCE)+34 <= r.h+14)
                data->line_top++;
            else
                data->line_pos++;

            data->line++;
        }
        else if (value == SDLK_LEFT && data->col > 0) { // <--
            data->pos--; data->col--;
            if (data->scroll > 0)
                data->scroll--;
        }
        else if (value == SDLK_RIGHT && data->pos < data->len-1) { // -->
            if (s[data->pos] == '\n') { // if "current char" = new line

                // if last line(DISPLAYED)
                if ( (data->line_pos*LINE_DISTANCE)+34 >= r.h-14 && (data->line_pos*LINE_DISTANCE)+34 <= r.h+14 )
                    data->line_top++;
                else
                    data->line_pos++;

                data->line++; data->col = 0; // CURSOR(Linha AZUL) no inicio da LINHA
            } else {
                data->col++;
                if (data->col*8 > r.w-88) {
                    data->scroll++;
//printf ("scroll: %d\n", data->scroll);
                }
            }

            data->pos++;
        }
        else if (value == SDLK_PAGEUP){ // PAGINA A CIMA: O mesmo que SETA A ACIMA * Numero de LINHAS VISIVEIS
            for (count=0; count < (r.h/LINE_DISTANCE)-2; count++)
            if (data->line > 0) {
                if ( data->line_pos > 0 ) data->line_pos--; else data->line_top--;
                data->line--;
            }
        }
        else if (value == SDLK_PAGEDOWN) {// PAGINA A BAIXO: O mesmo que SETA A BAIXO * Numero de LINHAS VISIVEIS
            for (count=0; count < (r.h/LINE_DISTANCE)-2; count++)
            if (data->line < data->line_count) {
                if ( (data->line_pos*LINE_DISTANCE)+34 >= r.h-14 && (data->line_pos*LINE_DISTANCE)+34 <= r.h+14)
                    data->line_top++;
                else
                    data->line_pos++;

                data->line++;
            }
        }
        else if (value == SDLK_DELETE && data->pos < data->len-1) {
            data->len--;  
            data->saved = 0;
            for (count = data->pos; data->text[count]; count++)
                data->text[count] = data->text[count+1];
        }
        else if (value == SDLK_BACKSPACE && data->col > 0) {
            data->saved = 0;
            data->pos--; data->col--; data->len--;
            for (count = data->pos; data->text[count]; count++)
                data->text[count] = data->text[count+1];
        }
        else if (value == SDLK_TAB && data->len < data->size-2) {
            data->saved = 0;
            InsertChar (data->text, data->pos, ' ');
            InsertChar (data->text, data->pos, ' ');
            data->col += 2; data->pos += 2; data->len += 2;
        }
        else
        //-----------------------------------------------------------
        // INSERT CHAR
        //-----------------------------------------------------------
        if (data->len < data->size-1 && (value==SDLK_RETURN || (value >= 32 && value <= 126))) {
            if (value==SDLK_RETURN) {
                InsertChar (data->text, data->pos, '\n');
              // if last line(DISPLAYED)
              //if ( (ED->line_pos*15)+15 >= O->h-12 && (ED->line_pos*15)+15 <= O->h+12 )
              if ( (data->line_pos*LINE_DISTANCE)+34 >= r.h-14 && (data->line_pos*LINE_DISTANCE)+34 <= r.h+14)
                  data->line_top++;
              else
                  data->line_pos++;

              data->line++; data->col = 0;
              data->text[ data->pos ] = '\n'; // New line: char 10
            } else {
                InsertChar (data->text, data->pos, value);
                data->col++; if (data->col*8 > r.w-40) data->scroll++;// Increment the CURSOR
            }

            data->len++;
            data->pos++; // Increment New POSITION
            data->saved = 0;
        }

        //--------------------------------------------------------------
        // ED->POS: < < < < < < < <     S T A R T     > > > > > > > > >
        // Get New position of: "ED->POS"
        //--------------------------------------------------------------
        data->line_ini = data->line_len = count = 0;

        //----------------------------------------------
        // line_ini
        // Get Position of START of LINE NOW
        //----------------------------------------------
        while (*str != 0 && count != data->line) {
            if (*str=='\n') count++;
            data->line_ini++;
            str++;
        }

        //----------------------------------------------
        // line_len
        // Get the SIZE of the LINE NOW
        //----------------------------------------------
        while (*str != 0 && *str != '\n'){
            data->line_len++;
            str++;
        }

        if (data->col > data->line_len)
            data->col = data->line_len;

        data->pos = data->line_ini + data->col; // <-- CODE HERE
        //-------------------------------------------------------------
        // ED->POS: < < < < < < < < <     E N D     > > > > > > > > > >
        // Get New position of: "ED->POS"
        //-------------------------------------------------------------

        if (value == SDLK_HOME) {
            data->pos = data->line_ini; data->col = 0;
        }
        else if (value == SDLK_END) {
            data->pos = data->line_ini + data->line_len;
            data->col = data->line_len;
            data->scroll = data->line_len + 70 - (r.w/8);
        }

        // if "data->col" NAO ULTRAPASSA O "tamanho->w"
        if (data->col*8 < r.w-88)
            data->scroll = 0;

        return RET_CALL;
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
    data->len = 0;
    if (text) {
        int i = 0;
        while (text[i]) {
            if (i >= size-2) break;
            data->text[i] = text[i];
            i++;
        }
        data->text[i] = 0;
        data->len = i;
    } else {
        data->text[0] = 0;
    }

    data->FileName[0] = 0;
    data->pos = 0;
    data->col = 0;
    data->scroll = 0;
    data->line = 0;
    data->line_top = 0;
    data->line_pos = 0;
    data->line_count = 0;
    data->size = size; // memory size text alloc
    data->color = COLOR_ORANGE;
    data->bg = 0;

    o = app_ObjectNew (proc_editor, x, y, 320, 245, id, OBJECT_TYPE_EDITOR, data);

    app_ObjectAdd (parent, o);

    return o;
}

