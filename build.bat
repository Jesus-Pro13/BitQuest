@echo off
nasm -f win64 .\rutinas.asm -o .\rutinas.obj
gcc -c .\main.c -o .\main.obj
gcc -c .\juego.c -o .\juego.obj
gcc .\main.obj .\juego.obj .\rutinas.obj -o .\BitQuest.exe

del .\main.obj .\juego.obj .\rutinas.obj

.\BitQuest.exe