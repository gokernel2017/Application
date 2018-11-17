//-------------------------------------------------------------------
//
// OBJECT Menu Implementation:
//
//-------------------------------------------------------------------
//
#include "app.h"

MENU * app_MenuCreate (int w, int h) {
    MENU *m;
    int i = 0, y = 0;
    if ((m = (MENU*) malloc (sizeof(MENU))) == NULL)
  return NULL;
    m->w = w;
    m->h = h;
    m->index = 0;
    m->top = 0;
    m->count = 0;
    m->button_h = 25;
    m->iten_first = NULL;
    // adjust size h
    for (i = 0; i < 100; i++) {
        if (y >= h) break;
        y += m->button_h;
    }
    m->h = (i * m->button_h)+5;
    return m;
}

void app_MenuItenAdd (MENU *m, char *text) {
    if (m && text) {
        MENU_ITEN *iten = (MENU_ITEN*) malloc (sizeof(MENU_ITEN));
        if (iten) {
            iten->text = strdup (text);
            iten->next = NULL;
            if (!m->iten_first) {
                m->iten_first = iten;
            } else {
                MENU_ITEN *aux = m->iten_first;
                while (aux->next != NULL) {
                    aux = aux->next;
                }
                aux->next = iten;
            }
            m->count++;
        }
    }
}

void app_MenuItenClear (MENU *m) {
    if (m) {
        MENU_ITEN *info;
        while (m->iten_first) {
            info = m->iten_first->next;

            if (m->iten_first->text)
                free (m->iten_first->text);

            free (m->iten_first);

            m->iten_first = info;
        }
        m->index = 0;
        m->top = 0;
        m->count = 0;
        m->iten_first = NULL;
    }
}

MENU_ITEN * app_MenuItenGet (MENU *m, int index) {
    int count = 0;
    MENU_ITEN *iten = m->iten_first;
    while (iten) {
        if (count++ == index)
      return iten;
        iten = iten->next;
    }
  return NULL;

}// app_MenuItenGet()


static void MenuDraw (MENU *m, int x, int y) {
    MENU_ITEN *iten;
    int pos_y = y, top = 0;
    SDL_FillRect (screen, &(SR){ x+1, y+1, m->w-2, m->h-2 }, MRGB(255,251,198)); // bg
    DrawRectR (screen, x, y, m->w, m->h, COLOR_ORANGE);
    iten = m->iten_first;
    while (iten) {
        if (pos_y > (y+m->h)-m->button_h) break;
        if (top >= m->top) {
            if (top == m->index) {
                SDL_FillRect (screen, &(SR){ x+2, pos_y+2, m->w-4, m->button_h }, MRGB(255,227,82)); // first iten
                DrawRect (screen, x+2, pos_y+2, m->w-5, m->button_h, COLOR_ORANGE); // border first iten
            }
            DrawText (screen, iten->text, x+10, pos_y+8, COLOR_ORANGE);
            pos_y += m->button_h;
        }
        top++;
        iten = iten->next;
    }
}

int app_Menu (MENU *m, int x, int y) {
    int quit = 0, ret = -1;
    if (!m) return -1;
    MenuDraw (m,x,y);
    SDL_UpdateRect (screen, x, y, m->w, m->h);
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_KEYDOWN) {
                int k;
                if ((k = e.key.keysym.unicode)==0)
                    k = e.key.keysym.sym;
                if (k == SDLK_ESCAPE) {
                    quit = 1;
                }
                else if (k == SDLK_RETURN) {
                    quit = 1;
                    ret = m->index;
                }
                else if (k == SDLK_UP && m->index > 0) {
                    m->index--;
                }
                else if (k == SDLK_DOWN && m->index < m->count-1) {
                    m->index++;
                }

                MenuDraw (m,x,y);
                SDL_UpdateRect (screen, x, y, m->w, m->h);
            }
            SDL_Delay(10);
        }
    }
    return ret;
}

