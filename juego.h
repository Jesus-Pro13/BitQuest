#ifndef JUEGO_H
#define JUEGO_H
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#define ROWS 60
#define COLUMNS 60

int findObject(char *matrix, int columns, int searchInRow, int searchInColumn, char character);

void imprimirMapa(int pY, int pX, char mapa[ROWS][COLUMNS]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int tamVentana = 20;
    int radio = 10;

    int inicioY = pY - radio;
    int inicioX = pX - radio;

    if (inicioY < 0) inicioY = 0;
    if (inicioX < 0) inicioX = 0;
    if (inicioY > 60 - tamVentana) inicioY = 60 - tamVentana;
    if (inicioX > 60 - tamVentana) inicioX = 60 - tamVentana;
    for(int i = inicioY; i < inicioY + tamVentana; i++) {
        for(int j = inicioX; j < inicioX + tamVentana; j++) {
            if (i==pY&&j==pX)
            {
                SetConsoleTextAttribute(hConsole, 11);
                printf("P");
            }else{
            
            char celda = mapa[i][j];
            
            if(celda == '#') SetConsoleTextAttribute(hConsole, 8);      
            else if(celda == 'M') SetConsoleTextAttribute(hConsole, 14); 
            else if(celda == 'K') SetConsoleTextAttribute(hConsole, 10); 
            else if(celda == 'D') SetConsoleTextAttribute(hConsole, 12);
            else SetConsoleTextAttribute(hConsole, 15);                
            
            printf("%c", celda);
            }
        }
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 7);

}

#endif