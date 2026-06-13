#include <stdio.h>
#include "juego.h"

int main()
{
    // llena la matriz de 1's
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLUMNS; j++)
        {
            matriz[i][j] = 1;
        }

    // cambia el 1 de (3, 6) por un 4
    matriz[3][6] = 4;

    // printf("hola mundo\n");

    // imprime el return (rax) de la funcion
    printf("%d\n", findObject(&matriz[0][0], COLUMNS, 3, 6, 4));
    return 0;
}
