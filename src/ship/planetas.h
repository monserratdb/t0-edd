typedef struct Planeta {
    int planetaID;
    int pedidos_entregados; 
} Planeta;

Planeta* crear_planeta(int planetaID);
void aumentar_pedidos_entregados(Planeta *planeta);
int comparar_planetas(const void *a, const void *b);
