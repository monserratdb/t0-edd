#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naves.h"
#include "planetas.h"
#include "pedidos.h"

static bool check_arguments(int argc, char **argv) {
    if (argc != 3) {
        printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
        printf("Donde:\n");
        printf("\tINPUT es la ruta del archivo de input\n");
        printf("\tOUTPUT es la ruta del archivo de output\n");
        exit(1);
    }
    return true;
}


void entregar_pedidos(Nave **naves, int num_naves, Pedido *pedidos, int *num_pedidos, Planeta **planetas, int num_planetas, FILE *output_file) {
    bool hay_pedidos_por_entregar = false;

    for (int i = 0; i < num_naves; i++) {
        Nave *nave_actual = naves[i];
        int pedido_prioritario = -1;
        for (int j = 0; j < *num_pedidos; j++) {
            Pedido pedido = pedidos[j];
            if (pedido.naveID == nave_actual->naveID) {
                pedido_prioritario = j;
                break;
            }
        }
        if (pedido_prioritario != -1) {
            hay_pedidos_por_entregar = true;
            int planeta_destino = pedidos[pedido_prioritario].planetaID;

            for (int j = 0; j < *num_pedidos; j++) {
                Pedido pedido = pedidos[j];
                if (pedido.naveID == nave_actual->naveID && pedido.planetaID == planeta_destino) {
                    fprintf(output_file, "PEDIDO %d ENTREGADO EN PLANETA %d\n", pedido.pedidoID, planeta_destino);
                    aumentar_pedidos_entregados(planetas[planeta_destino]);
                    //eliminar el entregado
                    for (int k = j; k < *num_pedidos - 1; k++) {
                        pedidos[k] = pedidos[k + 1];
                    }
                    (*num_pedidos)--;
                    j--; 
                }
            }
        }
    }
    if (!hay_pedidos_por_entregar) {
        fprintf(output_file, "NO HAY PEDIDOS POR ENTREGAR\n");
    }
}

void reporte_planetas(Planeta **planetas, int num_planetas, FILE *output_file) {
    Planeta **planetas_copia = malloc(num_planetas * sizeof(Planeta *));
    if (planetas_copia == NULL) {
        printf("Error de memoria\n");
        exit(1);
    }
    for (int i = 0; i < num_planetas; i++) {
        planetas_copia[i] = planetas[i];
    }

    qsort(planetas_copia, num_planetas, sizeof(Planeta *), comparar_planetas);

    fprintf(output_file, "PLANETAS-ORDENADOS\n");
    //bool algun_planeta = false; 
    for (int i = 0; i < num_planetas; i++) {
        fprintf(output_file, "    PLANETA %d: %d pedidos\n", planetas_copia[i]->planetaID, planetas_copia[i]->pedidos_entregados);
        //algun_planeta = true;
    }

    int total_pedidos_entregados = 0;
    for (int i = 0; i < num_planetas; i++) {
        total_pedidos_entregados += planetas_copia[i]->pedidos_entregados;
    }
    fprintf(output_file, "TOTAL-PEDIDOS-ENTREGADOS: %d\n", total_pedidos_entregados);
    free(planetas_copia);
}


int main(int argc, char **argv) {
    check_arguments(argc, argv);

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error al abrir los archivos de entrada/salida\n");
        exit(1);
    }

    int N_PLANETAS;
    int N_NAVES;
    int N_EVENTOS;

    int result;
    result = fscanf(input_file, "%d", &N_PLANETAS);
    if (result != 1) {
        printf("Error leyendo la cantidad de planetas\n");
        exit(1);
    }
    result = fscanf(input_file, "%d", &N_NAVES);
    if (result != 1) {
        printf("Error leyendo la cantidad de naves\n");
        exit(1);
    }
    result = fscanf(input_file, "%d", &N_EVENTOS);
    if (result != 1) {
        printf("Error leyendo la cantidad de eventos\n");
        exit(1);
    }

    char command[32];

    Nave **naves = malloc(N_NAVES * sizeof(Nave *));
    if (naves == NULL) {
        printf("Error de memoria\n");
        exit(1);
    }

    Planeta **planetas = malloc(N_PLANETAS * sizeof(Planeta *));
    if (planetas == NULL) {
        printf("Error de memoria\n");
        exit(1);
    }

    Pedido *pedidos = NULL;
    int num_pedidos = 0;

    for (int i = 0; i < N_NAVES; i++) {
        naves[i] = crear_nave(i);
        if (naves[i] == NULL) {
            printf("Error creando nave\n");
            exit(1);
        }
    }

    for (int i = 0; i < N_PLANETAS; i++) {
        planetas[i] = crear_planeta(i);
        if (planetas[i] == NULL) {
            printf("Error creando planeta\n");
            exit(1);
        }
    }

    while (fscanf(input_file, "%s", command) != EOF) {
        if (strcmp(command, "REGISTRAR-PEDIDO") == 0) {
            int pedidoID, naveID, planetaID;
            result = fscanf(input_file, "%d %d %d", &pedidoID, &naveID, &planetaID);
            if (result != 3) {
                printf("Error leyendo el pedido\n");
                exit(1);
            }
            registrar_pedido(&pedidos, &num_pedidos, pedidoID, naveID, planetaID);
            fprintf(output_file, "REGISTRADO PEDIDO %d\n", pedidoID);
        } else if (strcmp(command, "REPORTE-NAVE") == 0) {
            int naveID;
            result = fscanf(input_file, "%d", &naveID);
            if (result != 1) {
                printf("Error leyendo el ID de la nave\n");
                exit(1);
            }
            int pedidos_pendientes = contar_pedidos_nave(pedidos, num_pedidos, naveID);
            if (pedidos_pendientes == 0) {
                fprintf(output_file, "LA NAVE %d NO TIENE PEDIDOS PENDIENTES\n", naveID);
            } else {
                                fprintf(output_file, "PEDIDOS %d PENDIENTES: %d\n", naveID, pedidos_pendientes);
            }
        } else if (strcmp(command, "REPORTE-PEDIDOS") == 0) {
            fprintf(output_file, "REPORTE-PEDIDOS\n");
            int total_pedidos = 0;
            for (int i = 0; i < N_NAVES; i++) {
                int pedidos_pendientes = contar_pedidos_nave(pedidos, num_pedidos, i);
                if (pedidos_pendientes > 0) {
                    fprintf(output_file, "    NAVE %d\n", i);
                    for (int j = 0; j < num_pedidos; j++) {
                        if (pedidos[j].naveID == i) {
                            fprintf(output_file, "        PEDIDO %d CON PLANETA %d\n", pedidos[j].pedidoID, pedidos[j].planetaID);
                        }
                    }
                    total_pedidos += pedidos_pendientes;
                }
            }
            fprintf(output_file, "TOTAL DE PEDIDOS: %d\n", total_pedidos);
        } else if (strcmp(command, "PEDIDO-CONTAMINADO") == 0) {
            int naveID, pedidoID;
            result = fscanf(input_file, "%d %d", &naveID, &pedidoID);
            if (result != 2) {
                printf("Error leyendo la identificación de la nave y el pedido\n");
                exit(1);
            }
            bool pedido_encontrado = false;
            for (int i = 0; i < num_pedidos; i++) {
                if (pedidos[i].naveID == naveID && pedidos[i].pedidoID == pedidoID) {
                    pedido_encontrado = true;
                    fprintf(output_file, "PEDIDO %d HA SIDO ELIMINADO\n", pedidoID);
                    for (int j = i; j < num_pedidos - 1; j++) {
                        pedidos[j] = pedidos[j + 1];
                    }
                    num_pedidos--;
                    break;
                }
            }
            if (!pedido_encontrado) {
                fprintf(output_file, "PEDIDO %d NO ENCONTRADO EN NAVE %d\n", pedidoID, naveID);
            }
        } else if (strcmp(command, "TOMAR-DESPUES-MAX") == 0) {
            int pedidoID, naveID, planetaID;
            result = fscanf(input_file, "%d %d %d", &pedidoID, &naveID, &planetaID);
            if (result != 3) {
                printf("Error leyendo el pedido\n");
                exit(1);
            }
            int max_pos = -1;
            int max_pedidoID = -1;
            for (int i = 0; i < num_pedidos; i++) {
                if (pedidos[i].naveID == naveID && pedidos[i].pedidoID > max_pedidoID) {
                    max_pedidoID = pedidos[i].pedidoID;
                    max_pos = i;
                }
            }
            if (max_pos == -1) {
                registrar_pedido(&pedidos, &num_pedidos, pedidoID, naveID, planetaID);
            } else {
                pedidos = realloc(pedidos, (num_pedidos + 1) * sizeof(Pedido));
                if (pedidos == NULL) {
                    printf("Error de memoria\n");
                    exit(1);
                }

                for (int i = num_pedidos; i > max_pos + 1; i--) {
                    pedidos[i] = pedidos[i - 1];
                }
                pedidos[max_pos + 1] = (Pedido){pedidoID, naveID, planetaID};
                num_pedidos++;
            }
            fprintf(output_file, "REGISTRADO PEDIDO %d\n", pedidoID);
        } else if (strcmp(command, "ENTREGAR-PEDIDOS") == 0) {
            entregar_pedidos(naves, N_NAVES, pedidos, &num_pedidos, planetas, N_PLANETAS, output_file);
        } else if (strcmp(command, "REPORTE-PLANETAS") == 0) {
            reporte_planetas(planetas, N_PLANETAS, output_file);
        } else if (strcmp(command, "INVERSO") == 0) {
            int naveID;
            result = fscanf(input_file, "%d", &naveID);
            if (result != 1) {
                printf("Error leyendo el ID de la nave\n");
                exit(1);
            }
            invertir_pedidos_nave(pedidos, num_pedidos, naveID);
        }
    }         

    fclose(input_file);
    fclose(output_file);

    // Liberar memoria
    for (int i = 0; i < N_NAVES; i++) {
        free(naves[i]);
    }
    free(naves);

    for (int i = 0; i < N_PLANETAS; i++) {
        free(planetas[i]);
    }
    free(planetas);

    free(pedidos);

    return 0;
}