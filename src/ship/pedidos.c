#include <stdio.h>
#include <stdlib.h>
#include "pedidos.h"

void registrar_pedido(Pedido **pedidos, int *num_pedidos, int pedidoID, int naveID, int planetaID) {
    *pedidos = realloc(*pedidos, (*num_pedidos + 1) * sizeof(Pedido));
    if (*pedidos == NULL) {
        printf("Error de memoria\n");
        exit(1);
    }

    Pedido nuevo_pedido = {pedidoID, naveID, planetaID};
    (*pedidos)[*num_pedidos] = nuevo_pedido;
    (*num_pedidos)++;
}

void reportar_pedidos_nave(FILE *output_file, Pedido *pedidos, int num_pedidos, int naveID) {
    int pedidos_pendientes = 0;
    for (int i = 0; i < num_pedidos; i++) {
        if (pedidos[i].naveID == naveID) {
            pedidos_pendientes++;
        }
    }

    if (pedidos_pendientes == 0) {
        fprintf(output_file, "LA NAVE %d NO TIENE PEDIDOS PENDIENTES\n", naveID);
    } else {
        fprintf(output_file, "PEDIDOS %d PENDIENTES: %d\n", naveID, pedidos_pendientes);
    }
}

int contar_pedidos_nave(Pedido *pedidos, int num_pedidos, int naveID) {
    int contador = 0;
    for (int i = 0; i < num_pedidos; i++) {
        if (pedidos[i].naveID == naveID) {
            contador++;
        }
    }
    return contador;
}

void invertir_pedidos_nave(Pedido *pedidos, int num_pedidos, int naveID) {
    Pedido *pedidos_nave = NULL;
    int num_pedidos_nave = 0;

    for (int i = 0; i < num_pedidos; i++) {
        if (pedidos[i].naveID == naveID) {
            pedidos_nave = realloc(pedidos_nave, (num_pedidos_nave + 1) * sizeof(Pedido));
            if (pedidos_nave == NULL) {
                printf("Error de memoria\n");
                exit(1);
            }
            pedidos_nave[num_pedidos_nave] = pedidos[i];
            num_pedidos_nave++;
        }
    }

    for (int i = 0; i < num_pedidos_nave / 2; i++) {
        Pedido temp = pedidos_nave[i];
        pedidos_nave[i] = pedidos_nave[num_pedidos_nave - i - 1];
        pedidos_nave[num_pedidos_nave - i - 1] = temp;
    }
    int index = 0;
    for (int i = 0; i < num_pedidos; i++) {
        if (pedidos[i].naveID == naveID) {
            pedidos[i] = pedidos_nave[index];
            index++;
        }
    }
    free(pedidos_nave);
}
