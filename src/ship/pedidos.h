typedef struct Pedido {
    int pedidoID;
    int naveID;
    int planetaID;
} Pedido;

void registrar_pedido(Pedido **pedidos, int *num_pedidos, int pedidoID, int naveID, int planetaID);
void reportar_pedidos_nave(FILE *output_file, Pedido *pedidos, int num_pedidos, int naveID);
int contar_pedidos_nave(Pedido *pedidos, int num_pedidos, int naveID);
void invertir_pedidos_nave(Pedido *pedidos, int num_pedidos, int naveID);