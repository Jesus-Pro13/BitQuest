bits 64
default rel

global findObject
global monedasTotales
section .text

findObject:
    ; índice = row * columns + column
    mov eax, r8d        ; eax = row
    imul eax, edx       ; eax = row * columns
    add eax, r9d        ; eax = + column

    ; dirección = base + índice * 4
    movsx rax, eax
    lea r10, [rcx + rax]  ; si le enviamos un char (1 byte) hay que cambiar "rax*4" a "rax"

    ; leer entero
    movzx eax, byte [r10]

    ; obtener character
    movzx r11d, byte [rsp + 40]

    ; compara
    cmp eax, r11d
    jne .notFound

    mov rax, 1
    ret

    .notFound:
    mov rax, 0
    ret
monedasTotales:
    xor eax, eax        ; contador = 0
    test edx, edx       ; si celdas es 0, terminamos
    jz .fin

    .ciclo:
    movzx r9d, byte [rcx]      ; leer el carácter actual
    cmp r9b, r8b        ; comparar con el carácter buscado
    jne .siguiente
    inc eax             ; si coincide, incrementar contador

    .siguiente:
    inc rcx             ; avanzar al siguiente carácter
    dec edx             ; decrementar celdas restantes
    jnz .ciclo
    .fin:
    ret

    
    
