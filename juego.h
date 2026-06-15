#ifndef JUEGO_H
#define JUEGO_H
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include "mapas.h"
#define ROWS 60
#define COLUMNS 60

int findObject(char *matrix, int columns, int searchInRow, int searchInColumn, char character);
int monedasTotales(char *matrix, int celdas, char character);
int validar_movimiento(char *matrix, int columns, int searchInRow, int searchInColumn, char character);
int contarCeldaslibres(char *matrix, int celdas);

void imprimirMapa(int pY, int pX, char mapa[ROWS][COLUMNS])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int tamVentana = 20;
    int radio = 10;

    int inicioY = pY - radio;
    int inicioX = pX - radio;

    if (inicioY < 0)
        inicioY = 0;
    if (inicioX < 0)
        inicioX = 0;
    if (inicioY > 60 - tamVentana)
        inicioY = 60 - tamVentana;
    if (inicioX > 60 - tamVentana)
        inicioX = 60 - tamVentana;
    for (int i = inicioY; i < inicioY + tamVentana; i++)
    {
        for (int j = inicioX; j < inicioX + tamVentana; j++)
        {
            if (i == pY && j == pX)
            {
                SetConsoleTextAttribute(hConsole, 11);
                printf("P");
            }
            else
            {

                char celda = mapa[i][j];

                if (celda == '#')
                    SetConsoleTextAttribute(hConsole, 8);
                else if (celda == 'M')
                    SetConsoleTextAttribute(hConsole, 14);
                else if (celda == 'K')
                    SetConsoleTextAttribute(hConsole, 10);
                else if (celda == 'D')
                    SetConsoleTextAttribute(hConsole, 12);
                else
                    SetConsoleTextAttribute(hConsole, 15);

                printf("%c", celda);
            }
        }
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 7);
}void juego()
{
    int playerY = 1;
    int playerX = 1;
    int puntaje = 0;
    int monedasRecogidas = 0;
    int modedastotales = 0;
    int pasos = 0;
    bool haveKey = false;
    int nivel = 1;
    char tecla = 'm';

    char (*mapaActual)[COLUMNS] = mapa1; 

    modedastotales = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
    int libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);

    while (tecla != 113 && tecla != EOF)
    {
        system("cls");
        printf("--- NIVEL %d ---\n", nivel);
        printf("Puntaje: %d\n", puntaje);
        printf("Monedas recogidas: %d\n", monedasRecogidas);
        printf("Monedas totales: %d\n", modedastotales);
        printf("Celdas libres: %d\n", libres);
        
        imprimirMapa(playerY, playerX, mapaActual);

        tecla = _getch();

    
        if (tecla == 'w' || tecla == 'W')
        {
          
            if (!findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'M')) {
                    monedasRecogidas++;
                    mapaActual[playerY - 1][playerX] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'K')) {
                    haveKey = true;
                    mapaActual[playerY - 1][playerX] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'D')) {
                    if(haveKey) {
                        mapaActual[playerY - 1][playerX] = '.'; 
                    } else {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue; 
                    }
                }
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'E')) {
                    printf("¡Has ganado el nivel %d!\n", nivel);
                    Sleep(2000);
                    nivel++;
                    
                    if(nivel == 2) {
                        mapaActual = mapa2;
                    } else if(nivel == 3) {
                        mapaActual = mapa3;
                    } else {
                        printf("¡Felicidades, has completado el juego!\n");
                        Sleep(3000);
                        return;
                    }
                    
                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    modedastotales = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue; 
                } 
                
                playerY--;
                pasos++;
            }
        }

        if (tecla == 's' || tecla == 'S')
        {
            if (!findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'M')) {
                    monedasRecogidas++;
                    mapaActual[playerY + 1][playerX] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'K')) {
                    haveKey = true;
                    mapaActual[playerY + 1][playerX] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'D')) {
                    if(haveKey) {
                        mapaActual[playerY + 1][playerX] = '.';
                    } else {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue;
                    }
                }
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'E')) {
                    printf("¡Has ganado el nivel %d!\n", nivel);
                    Sleep(2000);
                    nivel++;
                    if(nivel == 2) {
                        mapaActual = mapa2;
                    } else if(nivel == 3) {
                        mapaActual = mapa3;
                    } else {
                        printf("¡Felicidades, has completado el juego!\n");
                        Sleep(3000);
                        return;
                    }
                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    modedastotales = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue;
                }
                
                playerY++;
                pasos++;
            }
        }
        if (tecla == 'a' || tecla == 'A')
        {
            if (!findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'M')) {
                    monedasRecogidas++;
                    mapaActual[playerY][playerX - 1] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'K')) {
                    haveKey = true;
                    mapaActual[playerY][playerX - 1] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'D')) {
                    if(haveKey) {
                        mapaActual[playerY][playerX - 1] = '.';
                    } else {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue;
                    }
                }
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'E')) {
                    printf("¡Has ganado el nivel %d!\n", nivel);
                    Sleep(2000);
                    nivel++;
                    if(nivel == 2) {
                        mapaActual = mapa2;
                    } else if(nivel == 3) {
                        mapaActual = mapa3;
                    } else {
                        printf("¡Felicidades, has completado el juego!\n");
                        Sleep(3000);
                        return;
                    }
                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    modedastotales = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue;
                }
                
                playerX--;
                pasos++;
            }
        }
        if (tecla == 'd' || tecla == 'D')
        {
            if (!findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'M')) {
                    monedasRecogidas++;
                    mapaActual[playerY][playerX + 1] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'K')) {
                    haveKey = true;
                    mapaActual[playerY][playerX + 1] = '.';
                }
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'D')) {
                    if(haveKey) {
                        mapaActual[playerY][playerX + 1] = '.';
                    } else {
                        printf("Necesitas la llave para abrir la puerta.\n");
                        Sleep(1000);
                        continue;
                    }
                }
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'E')) {
                    printf("¡Has ganado el nivel %d!\n", nivel);
                    Sleep(2000);
                    nivel++;
                    if(nivel == 2) {
                        mapaActual = mapa2;
                    } else if(nivel == 3) {
                        mapaActual = mapa3;
                    } else {
                        printf("¡Felicidades, has completado el juego!\n");
                        Sleep(3000);
                        return;
                    }
                    playerY = 1;
                    playerX = 1;
                    haveKey = false;
                    modedastotales = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
                    libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);
                    continue;
                }
                
                playerX++;
                pasos++;
            }
        }
    }
}

#endif