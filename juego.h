// Evitamos que este archivo de cabecera se incluya mas de una vez y cause errores
#ifndef JUEGO_H
#define JUEGO_H
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include "mapas.h" // Aqui traemos las tres matrices de nuestros niveles

// Definimos el tamano fijo de nuestros mapas (60x60)
#define ROWS 60
#define COLUMNS 60

// Declaramos las funciones de ensamblador (NASM) que leen la memoria directo
int findObject(char *matrix, int columns, int searchInRow, int searchInColumn, char character);
int monedasTotales(char *matrix, int celdas, char character);
int validar_movimiento(char *matrix, int columns, int searchInRow, int searchInColumn, char character);
int contarCeldaslibres(char *matrix, int celdas);

// Funcion externa que calculara el puntaje final
int score(int monedasRecogidas, int pasos, int nivelesCompletados);

// Variables globales para guardar el progreso historico de toda la partida
int monedasRecogidasTotal = 0;
int pasosTotales = 0;

// Esta funcion actua como la camara del juego. En lugar de imprimir la matriz de 60x60 completa,
// solo renderiza un cuadro de 20x20 caracteres alrededor del jugador para dar el efecto de exploracion.
void imprimirMapa(int pY, int pX, char mapa[ROWS][COLUMNS])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int tamVentana = 20;
    int radio = 10; // El jugador estara al centro de la vista

    // Calculamos las coordenadas donde empezara a dibujar la camara
    int inicioY = pY - radio;
    int inicioX = pX - radio;

    // Ponemos limites para que la camara no intente leer fuera de la matriz si nos pegamos a un borde
    if (inicioY < 0)
        inicioY = 0;
    if (inicioX < 0)
        inicioX = 0;
    if (inicioY > 60 - tamVentana)
        inicioY = 60 - tamVentana;
    if (inicioX > 60 - tamVentana)
        inicioX = 60 - tamVentana;

    // Ciclo doble para imprimir el pedazo de mapa visible fila por fila y columna por columna
    for (int i = inicioY; i < inicioY + tamVentana; i++)
    {
        for (int j = inicioX; j < inicioX + tamVentana; j++)
        {
            // Si la coordenada actual coincide con el jugador, imprimimos la 'P' en color azul claro
            if (i == pY && j == pX)
            {
                SetConsoleTextAttribute(hConsole, 11);
                printf("P");
            }
            else
            {
                // Si no es el jugador, leemos que hay en esa celda y le asignamos su color
                char celda = mapa[i][j];
                if (celda == '#')
                    SetConsoleTextAttribute(hConsole, 8); // Muros grises
                else if (celda == 'M')
                    SetConsoleTextAttribute(hConsole, 14); // Monedas amarillas
                else if (celda == 'K')
                    SetConsoleTextAttribute(hConsole, 10); // Llave verde
                else if (celda == 'D')
                    SetConsoleTextAttribute(hConsole, 12); // Puerta roja
                else
                    SetConsoleTextAttribute(hConsole, 15); // Espacios en blanco

                printf("%c", celda);
            }
        }
        printf("\n"); // Salto de linea al terminar cada fila de la vista
    }
    SetConsoleTextAttribute(hConsole, 7); // Regresamos la consola a su color normal
}

// Muestra el resumen rapido despues de terminar el nivel 1 o 2
void resumen(int nivel, int recogidasNivel, int totalesNivel, int pasosNivel)
{
    printf("\n--- Resumen del Nivel %d ---\n", nivel);
    printf("Monedas recogidas: %d / %d\n", recogidasNivel, totalesNivel);
    printf("Pasos dados      : %d\n", pasosNivel);
}

// Muestra las estadisticas globales al terminar todos los niveles o salir del juego
void resumenFinal(int nivel, int recogidasTotal, int totalesJuego, int pasosTotales)
{
    printf("\n=============================\n");
    printf("--   ! RESUMEN FINAL !     --\n");
    printf("=============================\n");
    printf("Niveles completados: %d\n", nivel - 1);
    printf("Monedas totales    : %d / %d\n", recogidasTotal, totalesJuego);
    printf("Pasos totales      : %d\n", pasosTotales);
    // Llama a la funcion score para calcular el puntaje segun el desempeno
    printf("Puntaje final      : %d\n", score(recogidasTotal, pasosTotales, nivel - 1));
}

// Ciclo principal del juego
void juego()
{
    // Usamos ensamblador para contar rapido cuantas monedas hay en los 3 mapas juntos (total historico)
    int monedasNivelesTotales = monedasTotales(&mapa1[0][0], ROWS * COLUMNS, 'M') + monedasTotales(&mapa2[0][0], ROWS * COLUMNS, 'M') + monedasTotales(&mapa3[0][0], ROWS * COLUMNS, 'M');

    // Variables locales para llevar el control del mapa actual. Se reinician en cada nivel.
    int monedasRecogidasNivel = 0;
    int pasosNivel = 0;
    int playerY = 1;
    int playerX = 1;
    bool haveKey = false;
    int nivel = 1;

    // Puntero clave: nos dice en que mapa de los 3 estamos jugando actualmente
    char (*mapaActual)[COLUMNS] = mapa1;

    // Calculamos datos del mapa actual para el HUD
    int monedasDelNivelActual = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
    int libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);

    char tecla = 'm';

    // Bucle del juego. Corre hasta presionar la tecla 'q' (ASCII 113) o error (EOF)
    while (tecla != 113 && tecla != EOF)
    {
        // system("cls"); // Limpia la terminal
        printf("\033[H"); // Limpia las terminal

        // Imprime el HUD con la informacion en tiempo real
        printf("--- NIVEL %d ---\n", nivel);
        printf("Llaves: %s\n", haveKey ? "Si" : "No");
        printf("Pasos: %d\n", pasosNivel);

        // Llamamos a la camara pasandole donde estamos y en que mapa
        imprimirMapa(playerY, playerX, mapaActual);

        // Espera a que el jugador presione una tecla
        tecla = _getch();

        // ---------------- MOVIMIENTO ARRIBA (W) ----------------
        if (tecla == 'w' || tecla == 'W')
        {
            // Valida en ensamblador que la celda de arriba (playerY - 1) NO sea muro ('#')
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, '#'))
            {
                // Si la celda destino es una moneda, la recogemos y borramos del mapa poniendo un punto
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'M'))
                {
                    monedasRecogidasNivel++;
                    mapaActual[playerY - 1][playerX] = '.';
                }
                // Si la celda destino es la llave, avisamos al sistema que ya la tenemos y la borramos
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'K'))
                {
                    haveKey = true;
                    mapaActual[playerY - 1][playerX] = '.';
                }
                // Logica de la puerta
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'D'))
                {
                    if (haveKey)
                    {
                        mapaActual[playerY - 1][playerX] = '.'; // Tienes llave, la puerta se abre (se borra)
                    }
                    else
                    {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue; // Corta la ejecucion aqui para evitar que el jugador avance atravesando la puerta
                    }
                }
                // Logica de transicion de nivel al tocar la salida 'E'
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'E'))
                {
                    // Traspasamos las monedas y pasos del nivel a nuestras variables historicas
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");

                    // Decidimos a que mapa mandarlo dependiendo del nivel
                    if (nivel == 1)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                        system("cls");
                    }
                    else if (nivel == 2)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                        system("cls");
                    }
                    else
                    {
                        // Si ya paso los niveles, mostramos resumen final y salimos del juego
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        system("cls");
                        nivel++;
                        resumenFinal(nivel, monedasRecogidasTotal, monedasNivelesTotales, pasosTotales);
                        Sleep(4000);
                        return;
                    }

                    // Si paso a un nivel nuevo, reseteamos al jugador y sus variables a cero
                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    monedasRecogidasNivel = 0;
                    pasosNivel = 0;
                    monedasDelNivelActual = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue;
                }

                // Si no choco con paredes ni puertas bloqueadas, movemos al personaje en el eje Y y sumamos el paso
                playerY--;
                pasosNivel++;
            }
        }

        // ---------------- MOVIMIENTO ABAJO (S) ----------------
        if (tecla == 's' || tecla == 'S')
        {
            // Valida celda destino abajo (playerY + 1)
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, '#'))
            {
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'M'))
                {
                    monedasRecogidasNivel++;
                    mapaActual[playerY + 1][playerX] = '.';
                }
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'K'))
                {
                    haveKey = true;
                    mapaActual[playerY + 1][playerX] = '.';
                }
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'D'))
                {
                    if (haveKey)
                    {
                        mapaActual[playerY + 1][playerX] = '.';
                    }
                    else
                    {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue;
                    }
                }
                if (findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'E'))
                {
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");

                    if (nivel == 1)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                        system("cls");
                    }
                    else if (nivel == 2)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                        system("cls");
                    }
                    else
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        system("cls");
                        nivel++;
                        resumenFinal(nivel, monedasRecogidasTotal, monedasNivelesTotales, pasosTotales);
                        Sleep(4000);
                        return;
                    }

                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    monedasRecogidasNivel = 0;
                    pasosNivel = 0;
                    monedasDelNivelActual = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue;
                }

                playerY++;
                pasosNivel++;
            }
        }

        // ---------------- MOVIMIENTO IZQUIERDA (A) ----------------
        if (tecla == 'a' || tecla == 'A')
        {
            // Valida celda destino izquierda (playerX - 1)
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), '#'))
            {
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'M'))
                {
                    monedasRecogidasNivel++;
                    mapaActual[playerY][playerX - 1] = '.';
                }
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'K'))
                {
                    haveKey = true;
                    mapaActual[playerY][playerX - 1] = '.';
                }
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'D'))
                {
                    if (haveKey)
                    {
                        mapaActual[playerY][playerX - 1] = '.';
                    }
                    else
                    {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue;
                    }
                }
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'E'))
                {
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");

                    if (nivel == 1)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                        system("cls");
                    }
                    else if (nivel == 2)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                        system("cls");
                    }
                    else
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        system("cls");
                        nivel++;
                        resumenFinal(nivel, monedasRecogidasTotal, monedasNivelesTotales, pasosTotales);
                        Sleep(4000);
                        return;
                    }

                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    monedasRecogidasNivel = 0;
                    pasosNivel = 0;
                    monedasDelNivelActual = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue;
                }

                playerX--;
                pasosNivel++;
            }
        }

        // ---------------- MOVIMIENTO DERECHA (D) ----------------
        if (tecla == 'd' || tecla == 'D')
        {
            // Valida celda destino derecha (playerX + 1)
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), '#'))
            {
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'M'))
                {
                    monedasRecogidasNivel++;
                    mapaActual[playerY][playerX + 1] = '.';
                }
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'K'))
                {
                    haveKey = true;
                    mapaActual[playerY][playerX + 1] = '.';
                }
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'D'))
                {
                    if (haveKey)
                    {
                        mapaActual[playerY][playerX + 1] = '.';
                    }
                    else
                    {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue;
                    }
                }
                if (findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'E'))
                {
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");

                    if (nivel == 1)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                        system("cls");
                    }
                    else if (nivel == 2)
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                        system("cls");
                    }
                    else
                    {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        system("cls");
                        nivel++;
                        resumenFinal(nivel, monedasRecogidasTotal, monedasNivelesTotales, pasosTotales);
                        Sleep(4000);
                        return;
                    }

                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    monedasRecogidasNivel = 0;
                    pasosNivel = 0;
                    monedasDelNivelActual = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue;
                }

                playerX++;
                pasosNivel++;
            }
        }
    }
    system("cls");
}
#endif