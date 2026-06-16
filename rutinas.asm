bits 64
default rel

global findObject
global monedasTotales
global validar_movimiento
global contarCeldaslibres
global score
section .text

; ========================================================================
; findObject: Busca un caracter especifico en una coordenada exacta
; Parametros de C: rcx = mapa, edx = columnas, r8d = fila, r9d = columna
; El 5to parametro (caracter a buscar) llega por la pila en [rsp + 40]
; ========================================================================
findObject:
    ; Calculamos el indice unidimensional: indice = (fila * columnas) + columna
    mov eax, r8d        ; eax = fila
    imul eax, edx       ; eax = fila * columnas
    add eax, r9d        ; eax = (fila * columnas) + columna

    ; Convertimos el indice de 32 a 64 bits para usarlo como desplazamiento de memoria
    movsx rax, eax
    
    ; Obtenemos la direccion exacta de la celda (base del mapa + indice)
    lea r10, [rcx + rax]  

    ; Leemos exactamente 1 byte (el char) de esa celda
    movzx eax, byte [r10]

    ; Leemos el 5to parametro desde la sombra de la pila (stack shadow space)
    movzx r11d, byte [rsp + 40]

    ; Comparamos el caracter del mapa con el que estamos buscando
    cmp eax, r11d
    jne .notFound       ; Si no son iguales, saltamos a notFound

    ; Si son iguales, retornamos 1 (verdadero)
    mov rax, 1
    ret

    .notFound:
    ; Si no son iguales, retornamos 0 (falso)
    mov rax, 0
    ret

; ========================================================================
; monedasTotales: Cuenta cuantas veces aparece un caracter en el mapa
; Parametros de C: rcx = mapa, edx = total de celdas, r8d = caracter ('M')
; ========================================================================
monedasTotales:
    xor eax, eax        ; Ponemos el contador general a 0
    test edx, edx       ; Verificamos si el total de celdas es 0
    jz .fin             ; Si es 0, terminamos de inmediato

    .ciclo:
    movzx r9d, byte [rcx]      ; Leemos 1 byte de la posicion actual del mapa
    cmp r9b, r8b        ; Comparamos byte contra byte con el caracter buscado
    jne .siguiente      ; Si no es el caracter, saltamos la suma
    inc eax             ; Si coincide, sumamos 1 a nuestro contador

    .siguiente:
    inc rcx             ; Movemos el puntero 1 byte hacia adelante (siguiente celda)
    dec edx             ; Restamos 1 al numero de celdas que faltan por revisar
    jnz .ciclo          ; Si aun faltan celdas (edx no es 0), repetimos el ciclo
    
    .fin:
    ret                 ; Retornamos el total acumulado en eax


; ========================================================================
; validar_movimiento: Revisa si el jugador puede pisar una celda
; Parametros de C: rcx = mapa, edx = columnas, r8d = fila nueva, r9d = columna nueva
; Devuelve 1 si es un espacio valido, 0 si es un muro '#'
; ========================================================================
validar_movimiento:
    ; Calculamos el indice lineal para la nueva posicion
    mov eax, r8d        ; eax = fila nueva
    imul eax, edx       ; eax = fila nueva * columnas
    add eax, r9d        ; eax = (fila nueva * columnas) + columna nueva

    ; Extendemos el resultado a 64 bits para el manejo de punteros
    movsx rax, eax

    ; Obtenemos el caracter exacto que hay en esa posicion
    mov r10b, byte [rcx + rax] 

    ; Verificamos si el caracter es un muro ('#')
    cmp r10b, '#'       
    je .movimiento_invalido ; Si es pared, saltamos y denegamos el paso

    ; Si no es pared, el paso es libre y retornamos 1
    mov rax, 1
    ret

.movimiento_invalido:
    ; Si hay un muro, retornamos 0 para bloquear el movimiento en C
    mov rax, 0
    ret

; ========================================================================
; contarCeldaslibres: Cuenta cuantos espacios vacios ('.') hay en total
; Parametros de C: rcx = mapa, edx = total de celdas
; Funciona casi identico a monedasTotales pero con el punto
; ========================================================================
contarCeldaslibres:
    xor eax, eax        ; Reiniciamos el contador a 0
    test edx, edx       ; Validamos que haya celdas por contar
    jz .fin             ; Si edx es 0, salimos directo

    .ciclo_contar:
    movzx r9d, byte [rcx]      ; Leemos el caracter de la memoria
    cmp r9b, '.'        ; Comparamos directamente contra el caracter '.' (espacio libre)
    jne .siguiente_contar      ; Si no es un punto, lo ignoramos
    inc eax             ; Si es un punto, sumamos 1 al contador

    .siguiente_contar:
    inc rcx             ; Avanzamos al siguiente byte de memoria
    dec edx             ; Le restamos 1 al total de celdas pendientes
    jnz .ciclo_contar   ; Repetimos hasta terminar todas las celdas
    
    .fin:
    ret                 ; Retornamos el total de puntos libres en eax

; ========================================================================
; score: Calcula la puntuacion final del jugador en base a su desempeno
; Formula: (monedas * 500) - (pasos * 5) + (nivel * 5000)
; Parametros de C: ecx = monedas, edx = pasos, r8d = nivel completado
; ========================================================================
score:
    xor rax, rax        ; Limpiamos rax para usar la parte inferior eax sin basura

    ; 1. Calculamos los puntos por monedas
    imul ecx, 500       ; Multiplicamos las monedas por 500
    add eax, ecx        ; Lo sumamos al total del puntaje (eax)

    ; 2. Restamos la penalizacion por dar muchos pasos
    imul edx, 5         ; Multiplicamos los pasos por 5
    sub eax, edx        ; Se lo restamos al total del puntaje (eax)

    ; 3. Sumamos el bono por niveles superados
    imul r8d, 5000      ; Multiplicamos los niveles completados por 5000
    add eax, r8d        ; Lo sumamos al puntaje final (eax)

    ; 4. Evitamos puntuaciones negativas forzando el minimo a 0
    cmp eax, 0          ; Revisamos si el puntaje total cayo por debajo de 0
    jge .noNeg          ; Si es mayor o igual a 0, saltamos al final

    mov eax, 0          ; Si dio negativo, lo forzamos a ser 0

    .noNeg:
    ret                 ; Retornamos el puntaje final calculado a C