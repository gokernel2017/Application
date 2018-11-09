#include "src/app.h"

#define ID_EDIT1    1000
#define ID_EDIT2    1001

OBJECT *ed;

void CreateInterface (void) {

    ed = app_NewEdit (NULL, ID_EDIT1, 20, 20, "Editor PRIMEIRO\n", 5000);

    app_NewEdit (NULL, ID_EDIT2, 20, 100, "Editor Segundo ... HELLO Isto eh um\n teste TESTANDO\n", 5000);

    app_NewButton (NULL, 0, 350, 50, "Button\n");
    
    app_SetFocus (ed);
}

int main (int argc, char **argv) {
    if (app_Init(argc,argv)) {
        CreateInterface ();
        app_Run (NULL);
    }
    printf ("Exiting With Sucess:\n");
    return 0;
}

