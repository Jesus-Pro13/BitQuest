#ifndef JUEGO_H
#define JUEGO_H
#include "mapas.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#define ROWS 60
#define COLUMNS 60

int findObject(char *matrix, int columns, int searchInRow, int searchInColumn, char character);

void imprimirMapa() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            char celda = mapa1[i][j];
            
            if(celda == '#') SetConsoleTextAttribute(hConsole, 8);      
            else if(celda == 'P') SetConsoleTextAttribute(hConsole, 11);
            else if(celda == 'M') SetConsoleTextAttribute(hConsole, 14); 
            else SetConsoleTextAttribute(hConsole, 15);                
            
            printf("%c", celda);
        }
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, 7);
}

#endif