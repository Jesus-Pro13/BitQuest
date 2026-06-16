Bienvenido a nuesto proyecto BitQuest, aqui encontraras las instrucciones para manejar el proyecto (no incluye documentacion de codigo)

requisitos:
    - SO: Windows 10 - Windows 11
    - Compilador: gcc (MSYS2 recomendado: https://github.com/msys2/msys2-installer/releases/download/2024-12-08/msys2-x86_64-20241208.exe) 
    - NASM obligatorio: https://www.nasm.us/pub/nasm/releasebuilds/3.01/win64/nasm-3.01-installer-x64.exe
    - Git necesario para descargar el repositorio: https://github.com/git-for-windows/git/releases/download/v2.54.0.windows.1/Git-2.54.0-64-bit.exe

forma de manejo:
    - compilacion:
        - abre tu terminal (PowerShell o CMD) y abre la carpeta donde descargaste el repositorio
        "Si no tienes descargado el repositorio, sigue las instrucciones abajo"
        - una vez estes dentro de la carpeta, puedes hacer ls -name y veras algo asi (en el explorador tambien veras .git):
            .vscode
            build.bat
            juego.h
            main.c
            main.c.txt  // No hace nada :v
            mapas.h
            README.txt
            rutinas.asm
        "recomendamos ignorar .vscode por temas de incompatibilidad"
        - ejecutar el comando .\build.bat (Solo compilara el proyecto)
    
    Cuando termine aparecera en la carpeta un nuevo archivo: BitQuest.exe (puedes comprobarlo con ls -Name)
    
    - ejecutar:
        - dentro de la misma carpeta, ejecutar BitQuest.exe
    
    Con esto ya puedes jugar BitQuest!

    Errores comunes:
        - Si te da errores de compilacion, asegurate de tener correctamente instalados los compiladores.
        - Si aun asi no funciona, revisa la documentacion para obtener mas informacion acerca del codigo.
        - Recuerda que necesitas estar en Windows 10 - 11 de 64 bits para ejecutar el programa correctamente.

descarga del repositorio:
    "si tienes experiencia con git puedes saltar hasta el checkout antes del pull, solo evita usar origin en las declaraciones que lo usan normalmente y usa main en su lugar"
    - inicia un repositorio en una carpeta nueva con: git init
    - agrega la fuente del repositorio con: git add remote main https://github.com/Jesus-Pro13/BitQuest.git
    - haz un fetch con: git fetch main
    - haz: gitcheckout -b main
    - git pull main main