bits 64
default rel

global findObject
global monedasTotales
global validar_movimiento
global contarCeldaslibres
global score
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

    
    

; funcion que valida el movimiento del jugador, recibe la posicion actual del jugador y la posicion a la que se quiere mover, devuelve 1 si el movimiento es valido y 0 si no
validar_movimiento:
    ;indice del jugador
    mov eax, r8d ;nueva fila
    imul eax, edx ;nueva fila * columnas
    add eax, r9d ;nueva fila * columnas + nueva columna

    ;conversion a 64 bits para el desplazamiento de memoria
    movsx rax, eax

    mov r10b, byte [rcx + rax] ;obtenemos el valor del mapa en la nueva posicion

    ;verificamos si es pared
    cmp r10b, '#' ;si es una pared, el movimiento no es valido
    je .movimiento_invalido

    ;si no es pared retorna valido 1
    mov rax, 1
    ret

.movimiento_invalido:
    mov rax, 0
    ret
contarCeldaslibres:
    xor eax, eax        ; contador = 0
    test edx, edx       ; si celdas es 0, terminamos
    jz .fin

    .ciclo_contar:
    movzx r9d, byte [rcx]      ; leer el carácter actual
    cmp r9b, '.'        ; comparar con el carácter buscado
    jne .siguiente_contar
    inc eax             ; si coincide, incrementar contador

    .siguiente_contar:
    inc rcx             ; avanzar al siguiente carácter
    dec edx             ; decrementar celdas restantes
    jnz .ciclo_contar
    .fin:
    ret
score:
 ; puntuacion=(monedas*valorMoneda)-(pasos*valorPasos)+(nivel*valorNiveles)
    
    xor rax, rax
    imul ecx, 500
    add eax, ecx
    imul edx, 5
    sub eax, edx
    imul r8d, 5000
    add eax, r8d

    cmp eax, 0
    jge .noNeg

    mov eax, 0

    .noNeg:
    ret