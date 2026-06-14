#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h> 

void mostrarPantallaInicio() {
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    

    system("cls");

    
    SetConsoleTextAttribute(hConsole, 10);
    
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                        BBBBBB   IIIIII  TTTTTT    QQQQ    U    U  EEEEEE   SSSSS   TTTTTT\n");
    printf("                        B     B    II      TT     Q    Q   U    U  E       S          TT  \n");
    printf("                        B     B    II      TT     Q    Q   U    U  E       S          TT  \n");
    printf("                        BBBBBB     II      TT     Q    Q   U    U  EEEEE    SSSSS     TT  \n");
    printf("                        B     B    II      TT     Q    Q   U    U  E             S    TT  \n");
    printf("                        B     B    II      TT     Q  Q Q   U    U  E             S    TT  \n");
    printf("                        BBBBBB   IIIIII    TT      QQQQ Q   UUUU   EEEEEE   SSSSS     TT  \n");
    printf("\n");
    printf("\n");

    int parpadeo = 1;

    
    while (!_kbhit()) {
        
        printf("\r"); 
        
        if (parpadeo) {
            
            SetConsoleTextAttribute(hConsole, 14);
            printf("\t\t\t\tPresione cualquier tecla para continuar...          ");
        } else {
            
            printf("\t\t\t                                                              ");
        }
        
        parpadeo = !parpadeo; 
        Sleep(500);           
    }

    _getch(); 

    SetConsoleTextAttribute(hConsole, 7);
    system("cls"); 
}

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; 
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    mostrarPantallaInicio();



    return 0;
}