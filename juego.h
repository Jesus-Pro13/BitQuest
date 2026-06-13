#ifndef JUEGO_H
#define JUEGO_H

#define ROWS 60
#define COLUMNS 60

int matriz[ROWS][COLUMNS];


int findObject(int *matrix, int columns, int searchInRow, int searchInColumn, int character);

#endif