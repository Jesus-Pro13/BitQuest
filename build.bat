@echo off
nasm -f win64 .\rutinas.asm -o .\rutinas.obj
gcc -c .\main.c -o .\main.obj
gcc .\main.obj .\rutinas.obj -o .\BitQuest.exe

del .\main.obj .\rutinas.obj