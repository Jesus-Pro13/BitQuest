bits 64
default rel

global findObject
section .text

findObject:
    ; índice = row * columns + column
    mov eax, r8d        ; eax = row
    imul eax, edx       ; eax = row * columns
    add eax, r9d        ; eax = + column

    ; dirección = base + índice * 4
    movsx rax, eax
    lea r10, [rcx + rax*4]  ; si le enviamos un char (1 byte) hay que cambiar "rax*4" a "rax"

    ; leer entero
    mov eax, [r10]

    ; obtener character
    mov r11d, [rsp + 40]

    ; compara
    cmp eax, r11d
    jne .notFound

    mov rax, 1
    ret

    .notFound:
    mov rax, 0
    ret