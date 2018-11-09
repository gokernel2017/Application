#include "src/sgui.h"

#define ID_EDITOR1    1000
#define ID_EDITOR2    1001

OBJECT *ed;

void CreateInterface (void) {

    ed = sg_NewEditor (NULL, ID_EDITOR1, 5, 5, "Editor PRIMEIRO\n", 5000);

    sg_NewEditor (NULL, ID_EDITOR2, 5, 320, "Editor Segundo ... HELLO Isto eh um\n teste TESTANDO\n", 5000);

    sg_NewButton (NULL, 0, 670, 100, "Button 01\n");
    
    sg_SetFocus (ed);
}

int main (int argc, char **argv) {
    if (sg_Init(argc,argv)) {
        CreateInterface ();
        sg_Run (NULL);
    }
    printf ("Exiting With Sucess:\n");
    return 0;
}

