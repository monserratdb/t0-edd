#include "planetas.h"
#include <stdio.h>
#include <stdlib.h>


Planeta* crear_planeta(int planetaID) {
    Planeta* nuevo_planeta = malloc(sizeof(Planeta));
    if (nuevo_planeta != NULL) {
        nuevo_planeta->planetaID = planetaID;
        nuevo_planeta->pedidos_entregados = 0;
    }
    return nuevo_planeta;
}

void aumentar_pedidos_entregados(Planeta *planeta) {
    planeta->pedidos_entregados++;
}


int comparar_planetas(const void *a, const void *b) {
    const Planeta *planetaA = *(const Planeta **)a;
    const Planeta *planetaB = *(const Planeta **)b;

    return planetaB->pedidos_entregados - planetaA->pedidos_entregados;
}
