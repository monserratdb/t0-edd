#include <stdio.h>
#include <stdlib.h>
#include "naves.h"

Nave* crear_nave(int naveID) {
    Nave* nueva_nave = malloc(sizeof(Nave));
    if (nueva_nave != NULL) {
        nueva_nave->naveID = naveID;
    }
    return nueva_nave;
}