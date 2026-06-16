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
int score(int monedasRecogidas, int pasos, int nivelesCompletados);
int monedasRecogidasTotal = 0;
int pasosTotales = 0;
void imprimirMapa(int pY, int pX, char mapa[ROWS][COLUMNS])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int tamVentana = 20;
    int radio = 10;

    int inicioY = pY - radio;
    int inicioX = pX - radio;

    if (inicioY < 0) inicioY = 0;
    if (inicioX < 0) inicioX = 0;
    if (inicioY > 60 - tamVentana) inicioY = 60 - tamVentana;
    if (inicioX > 60 - tamVentana) inicioX = 60 - tamVentana;
    
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
                if (celda == '#') SetConsoleTextAttribute(hConsole, 8);
                else if (celda == 'M') SetConsoleTextAttribute(hConsole, 14);
                else if (celda == 'K') SetConsoleTextAttribute(hConsole, 10);
                else if (celda == 'D') SetConsoleTextAttribute(hConsole, 12);
                else SetConsoleTextAttribute(hConsole, 15);

                printf("%c", celda);
            }
        }
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, 7);
}

void resumen(int nivel, int recogidasNivel, int totalesNivel, int pasosNivel) {
    printf("\n--- Resumen del Nivel %d ---\n", nivel);
    printf("Monedas recogidas: %d / %d\n", recogidasNivel, totalesNivel);
    printf("Pasos dados      : %d\n", pasosNivel);
}

void resumenFinal(int nivel, int recogidasTotal, int totalesJuego, int pasosTotales) {
    printf("\n=============================\n");
    printf("--   ! RESUMEN FINAL !     --\n");
    printf("=============================\n");
    printf("Niveles completados: %d\n", nivel - 1);
    printf("Monedas totales    : %d / %d\n", recogidasTotal, totalesJuego);
    printf("Pasos totales      : %d\n", pasosTotales);
    printf("Puntaje final      : %d\n", score(recogidasTotal, pasosTotales, nivel - 1));
}

void juego(){
    int monedasNivelesTotales = monedasTotales(&mapa1[0][0], ROWS * COLUMNS, 'M') + monedasTotales(&mapa2[0][0], ROWS * COLUMNS, 'M') + monedasTotales(&mapa3[0][0], ROWS * COLUMNS, 'M');
    int monedasRecogidasNivel = 0;
    int pasosNivel = 0;
    int playerY = 1;
    int playerX = 1;
    bool haveKey = false;
    int nivel = 1;
    
    char (*mapaActual)[COLUMNS] = mapa1; 
    int monedasDelNivelActual = monedasTotales(&mapaActual[0][0], ROWS * COLUMNS, 'M');
    int libres = contarCeldaslibres(&mapaActual[0][0], ROWS * COLUMNS);

    char tecla = 'm';

    while (tecla != 113 && tecla != EOF)
    {
        system("cls");
        printf("--- NIVEL %d ---\n", nivel);
        printf("Llaves: %s\n", haveKey ? "Si" : "No");
        printf("Pasos: %d\n", pasosNivel);
        
        imprimirMapa(playerY, playerX, mapaActual);

        tecla = _getch();

        if (tecla == 'w' || tecla == 'W')
        {
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY - 1), playerX, 'M')) { 
                    monedasRecogidasNivel++;
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
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");
                    
                    if(nivel == 1) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                    } else if(nivel == 2) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                    } else {
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
                
                playerY--;
                pasosNivel++;
            }
        }

        if (tecla == 's' || tecla == 'S')
        {
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, (playerY + 1), playerX, 'M')) {
                    monedasRecogidasNivel++;
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
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");
                    
                    if(nivel == 1) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                    } else if(nivel == 2) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                    } else {
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

        if (tecla == 'a' || tecla == 'A')
        {
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX - 1), 'M')) {
                    monedasRecogidasNivel++;
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
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");
                    
                    if(nivel == 1) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                    } else if(nivel == 2) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                    } else {
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

        if (tecla == 'd' || tecla == 'D')
        {
            if (validar_movimiento(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), '#'))
            {
                if(findObject(&mapaActual[0][0], COLUMNS, playerY, (playerX + 1), 'M')) {
                    monedasRecogidasNivel++;
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
                    monedasRecogidasTotal += monedasRecogidasNivel;
                    pasosTotales += pasosNivel;

                    Sleep(1000);
                    system("cls");
                    
                    if(nivel == 1) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa2;
                    } else if(nivel == 2) {
                        resumen(nivel, monedasRecogidasNivel, monedasDelNivelActual, pasosNivel);
                        Sleep(3000);
                        nivel++;
                        mapaActual = mapa3;
                    } else {
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
}
#endif