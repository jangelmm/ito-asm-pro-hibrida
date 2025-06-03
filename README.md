# Documentación
## Manual de Usuario y Desarrollo: HYBDEMO con DOSBox y Turbo C++

Este manual detalla el proceso para instalar DOSBox en un sistema Ubuntu, configurar el entorno de desarrollo con Turbo C++ y compilar y ejecutar el programa `HYBDEMO.CPP`. Adicionalmente, se explica el funcionamiento del programa y se proporcionan ejemplos de valores de entrada para observar su comportamiento.

-----

## 1\. Instalación de DOSBox en Ubuntu

Sigue estos pasos en una terminal de Ubuntu para instalar DOSBox:

1.  **Actualizar el sistema:**
    ```bash
    sudo apt update && sudo apt upgrade
    ```
2.  **Instalar DOSBox:**
    ```bash
    sudo apt-get install dosbox
    ```
3.  **Ejecutar DOSBox por primera vez:**
    Para verificar la instalación y permitir que DOSBox cree su archivo de configuración inicial, ejecuta:
    ```bash
    dosbox
    ```
    Se abrirá la ventana de DOSBox. Puedes cerrarla escribiendo `exit` en el prompt `Z:\>` y presionando Enter.

-----

## 2\. Configuración del Entorno de Desarrollo

### 2.1. Estructura de Directorios y Archivos Necesarios

Asegúrate de tener la siguiente estructura de directorios y archivos en tu sistema. Basado en tu información, se asume que la carpeta principal del proyecto es `MASM`, ubicada en `/home/angel/Documents/asm/`.

```
/home/angel/Documents/asm/
└── MASM/                 <-- Este directorio se montará como C:
    ├── HYBDEMO.CPP       <-- Tu archivo fuente principal
    ├── TC/                 <-- Directorio de Turbo C++
    │   ├── BIN/            <-- Contiene TCC.EXE (compilador)
    │   ├── INCLUDE/        <-- Archivos de cabecera (.h)
    │   ├── LIB/            <-- Archivos de librería (.LIB)
    │   └── ... (otros subdirectorios y archivos de TC)
    ├── tc3.zip             <-- Archivo ZIP con la instalación de Turbo C++ 3.0 (si TC/ no está completo)
    └── ... (otros archivos como MASM.EXE, DEBUG.EXE, etc.)
```

  * **HYBDEMO.CPP**: Tu archivo de código fuente. Ya lo tienes en `MASM/`.
  * **Turbo C++ (TC)**: El compilador. Ya tienes un directorio `TC/` dentro de `MASM/` que contiene `BIN/`, `INCLUDE/` y `LIB/`. Si este directorio no estuviera completo, deberías descomprimir `tc3.zip` de tal manera que la estructura `TC/BIN`, `TC/INCLUDE`, `TC/LIB` quede correctamente poblada dentro de `MASM/TC/`.
    Tu entrada indica: *"En su carpeta de Documentos descomprimen el `archivo.zip`"*
    Asumiendo que `archivo.zip` es `tc3.zip` y contiene la estructura de Turbo C++, si necesitaras restaurar o instalar Turbo C++ en el directorio `MASM/TC/`:
      * En Ubuntu, navega a `/home/angel/Documents/asm/MASM/`.
      * Si `TC/` necesita ser (re)poblado: `unzip tc3.zip -d TC` (esto podría requerir crear `TC` primero o borrar su contenido si ya existe y está corrupto). Sin embargo, tu `tree` muestra que `MASM/TC/` ya está bien estructurado.

### 2.2. Configuración de Montaje y Arranque Automático en DOSBox

Para que DOSBox monte automáticamente tu directorio de trabajo y configure el teclado:

1.  **Localiza el archivo de configuración de DOSBox**:
    Este archivo se encuentra en `/home/TU_USUARIO/.dosbox/`. El nombre exacto puede variar ligeramente (ej: `dosbox-0.74-3.conf`). Reemplaza `TU_USUARIO` con tu nombre de usuario real (en tu caso, `angel`).

      * Si no estás seguro del nombre, puedes abrir DOSBox y escribir `config -writeconf archivo_conf_dosbox` y luego `exit`. Esto creará `archivo_conf_dosbox` en tu carpeta personal de Ubuntu, que puedes usar como referencia o editar directamente.

2.  **Edita el archivo de configuración**:
    Abre el archivo `.conf` con un editor de texto (ej. `gedit /home/angel/.dosbox/dosbox-0.74-3.conf`).

3.  **Añade las líneas de auto-ejecución**:
    Al final del archivo, busca la sección `[autoexec]`. Si no existe, créala. Agrega las siguientes líneas:

    ```ini
    [autoexec]
    # Líneas en esta sección se ejecutarán al inicio.
    @ECHO OFF
    keyb la
    mount c /home/angel/Documents/asm/MASM
    c:
    PATH C:\TC\BIN;%PATH%
    ECHO Bienvenido a DOSBox con Turbo C++ y MASM.
    ECHO Unidad C: montada en /home/angel/Documents/asm/MASM
    ECHO HYBDEMO.CPP debe estar en C:\
    ECHO Compilador Turbo C++ (TCC.EXE) accesible desde el PATH.
    ```

    **Importante**:

      * Reemplaza `angel` con tu nombre de usuario si es diferente.
      * `keyb la`: Configura el teclado para Latinoamérica.
      * `mount c /home/angel/Documents/asm/MASM`: Hace que tu directorio local `MASM` sea la unidad `C:` dentro de DOSBox.
      * `c:`: Cambia a la unidad `C:` como activa.
      * `PATH C:\TC\BIN;%PATH%`: Añade el directorio `BIN` de Turbo C++ al `PATH` de DOSBox. Esto te permitirá ejecutar `TCC.EXE` desde cualquier lugar en la unidad `C:`, sin necesidad de navegar a `C:\TC\BIN\`.

4.  **Guarda** el archivo de configuración.

-----

## 3\. Compilación del Programa `HYBDEMO.CPP`

Con DOSBox y el entorno configurado:

1.  **Inicia DOSBox**:
    Abre una terminal de Ubuntu y ejecuta:

    ```bash
    dosbox
    ```

    Si la configuración de `autoexec` es correcta, DOSBox iniciará y te dejará en el prompt `C:\>`.

2.  **Verifica la ubicación del archivo**:
    Tu archivo `HYBDEMO.CPP` ya está en `C:\` (que es `/home/angel/Documents/asm/MASM/`).

3.  **Comando de Compilación**:
    En el prompt `C:\>` de DOSBox, escribe el siguiente comando para compilar tu programa:

    ```dos
    TCC -ml -IC:\TC\INCLUDE -LC:\TC\LIB C:\HYBDEMO.CPP
    ```

      * `TCC`: Ejecutable del compilador Turbo C++.
      * `-ml`: Modelo de memoria "large". Es adecuado para tu programa.
      * `-IC:\TC\INCLUDE`: Especifica la ruta a los archivos de cabecera (`.h`) de Turbo C++.
      * `-LC:\TC\LIB`: Especifica la ruta a las bibliotecas (`.LIB`) de Turbo C++.
      * `HYBDEMO.CPP`: Tu archivo fuente.

    La imagen que proporcionaste muestra una advertencia durante la compilación:
    `Warning C:\hybdemo.cpp 519: 'page_t' is assigned a value That is never used in function menu_pantalla_dos()`
    Esto es solo una advertencia indicando que a la variable `page_t` se le asigna un valor pero luego no se utiliza en esa función. El programa compilará y enlazará correctamente, generando `HYBDEMO.EXE`.

4.  **Verifica la salida**:
    Si la compilación es exitosa, no verás errores (solo la advertencia mencionada). En tu directorio `C:\` (es decir, `/home/angel/Documents/asm/MASM/` en Ubuntu) aparecerá el archivo `HYBDEMO.EXE`.

-----

## 4\. Ejecución y Uso del Programa `HYBDEMO.EXE`

### 4.1. Ejecutar el Programa

Una vez compilado, desde el prompt `C:\>` en DOSBox, ejecuta el programa escribiendo:

```dos
HYBDEMO.EXE
```

El programa se iniciará y mostrará el menú principal.

### 4.2. Guía de Uso del Programa (Valores de Ejemplo)

El programa te permite interactuar con diferentes módulos de hardware mediante interrupciones del BIOS/DOS. Aquí tienes ejemplos de qué valores ingresar:

**Menú Principal:**

```
SISTEMA DE CONTROL DE HARDWARE (DOSBox - INT Reales)
===================================================
1. PANTALLA (INT 10h)
2. DISCO (INT 13h)
3. RATON (INT 33h)
4. IMPRESORA (INT 17h)
5. SALIR

Seleccione una opcion:
```

#### **Opción 1: PANTALLA (INT 10h)**

Al seleccionar `1`, accederás al submenú de pantalla:

```
--- MODULO PANTALLA (DOS - INT 10h) ---
1. Establecer Modo Video (AH=00h)
2. Establecer Tipo Cursor (AH=01h)
3. Posicionar Cursor (AH=02h)
4. Obtener Posicion Cursor (AH=03h)
5. Escribir Caracter/Atributo (AH=09h)
6. Limpiar Pantalla (con clrscr)
7. Scroll Ventana Arriba (AH=06h)
0. Volver al Menu Principal
```

  * **1. Establecer Modo Video:**
      * `Modo de video (hex):` ingresa `3` (o `03`). Esto establece el modo texto 80x25, 16 colores.
      * Otro ejemplo: `12` (hex) para modo gráfico VGA 640x480, 16 colores.
  * **2. Establecer Tipo Cursor:**
      * `Linea inicio cursor (scanline):` `6` o `2`
      * `Linea fin cursor (scanline):` `7` o `10` (Esto crea un cursor de bloque en modo texto).
  * **3. Posicionar Cursor:**
      * `Fila:` `10`
      * `Columna:` `20`
  * **4. Obtener Posicion Cursor:** No requiere entrada, solo muestra la información.
  * **5. Escribir Caracter/Atributo:**
      * `Caracter:` `H`
      * `Atributo (hex):` `1E` (Texto amarillo brillante sobre fondo azul).
          * *Formato del atributo: (COLOR\_FONDO \<\< 4) | COLOR\_TEXTO. Colores: 0-Negro, 1-Azul, 2-Verde, 3-Cyan, 4-Rojo, 5-Magenta, 6-Marrón, 7-Gris Claro. Añade 8 para alta intensidad (ej. $0xF$ = Blanco brillante).*
      * `Veces (dec):` `5` (Escribirá 'HHHHH').
  * **6. Limpiar Pantalla:** No requiere entrada.
  * **7. Scroll Ventana Arriba:**
      * `Lineas a scrollear (ej.1):` `2`
      * `Atributo para lineas nuevas (hex, ej.07):` `07` (Gris claro sobre negro). `1F` (Color Azul)

#### **Opción 2: DISCO (INT 13h)**

```
--- MODULO DISCO (DOS - INT 13h) ---
ADVERTENCIA: Operaciones de disco pueden ser peligrosas!
1. Resetear Sistema de Disco (AH=00h)
2. Obtener Estado de Disco (AH=01h)
3. Leer Sector de Prueba (AH=02h) del drive actual
0. Volver al Menu Principal
Drive actual para pruebas: 0x80
```

  * **1. Resetear Sistema de Disco:**
      * `Drive a resetear (hex, 00=A:, 80=C:, actual=0x80):` `80` (para la unidad C:).
  * **2. Obtener Estado de Disco:**
      * `Drive para obtener estado (hex, actual=0x80):` `80`.
  * **3. Leer Sector de Prueba:** Lee el sector 1, C=0, H=0 del drive `0x80`. Mostrará los primeros 16 bytes leídos. No requiere entrada adicional.

#### **Opción 3: RATON (INT 33h)**

*Nota: El ratón debe ser capturado por DOSBox para que funcione (haz clic dentro de la ventana de DOSBox). Presiona `CTRL+F10` para liberar el ratón.*

```
--- MODULO RATON (DOS - INT 33h) ---
1. Mostrar Cursor Mouse (AX=0001h)
2. Ocultar Cursor Mouse (AX=0002h)
3. Obtener Posicion/Botones (AX=0003h)
4. Establecer Posicion Cursor (AX=0004h)
0. Volver al Menu Principal
```

  * **1. Mostrar Cursor Mouse:** No requiere entrada.
  * **2. Ocultar Cursor Mouse:** No requiere entrada.
  * **3. Obtener Posicion/Botones:** No requiere entrada. Mueve el ratón y haz clic para ver los cambios en la información mostrada.
  * **4. Establecer Posicion Cursor:**
      * `Nueva X (0-639):` `100`
      * `Nueva Y (0-199 o 0-349 para texto):` `50`

#### **Opción 4: IMPRESORA (INT 17h)**

*Nota: La salida de la impresora en DOSBox puede ser redirigida a un archivo o ser ignorada, dependiendo de la configuración de DOSBox.*

```
--- MODULO IMPRESORA (DOS - INT 17h) ---
1. Inicializar Impresora (AH=01h)
2. Obtener Estado Impresora (AH=02h)
3. Enviar Caracter a Impresora (AH=00h)
0. Volver al Menu Principal
```

  * **1. Inicializar Impresora:**
      * `Numero impresora (0=LPT1, 1=LPT2, 2=LPT3):` `0`
  * **2. Obtener Estado Impresora:**
      * `Numero impresora (0-2):` `0`
  * **3. Enviar Caracter a Impresora:**
      * `Numero impresora (0-2):` `0`
      * `Caracter a enviar:` `P`

#### **Opción 5: SALIR**

Termina la ejecución del programa `HYBDEMO.EXE`.

-----

## 5\. Explicación del Código `HYBDEMO.CPP` )

Tu código `HYBDEMO.CPP` está estructurado en varios módulos, cada uno manejando interacciones específicas con el hardware a través de interrupciones del BIOS o DOS.

  * **Inclusiones**:

      * `iostream.h`: Para entrada/salida estándar (ej. `cout`, `cin`).
      * `iomanip.h`: Para manipulación del formato de salida (ej. `setw`, `hex`).
      * `conio.h`: Para funciones de consola como `clrscr()` (limpiar pantalla) y `getch()` (obtener caracter).
      * `dos.h`: Para funciones específicas de DOS, como `FP_SEG` y `FP_OFF` (usadas para obtener el segmento y desplazamiento de punteros `far`, necesarios para la interrupción de lectura de disco).

  * **Estructuras**: `DiskStatus` y `PrinterStatus` se usan para devolver el resultado de las operaciones de disco e impresora de forma organizada.

  * **Prototipos de Funciones**: Declaran todas las funciones que se usarán, agrupadas por módulos (Pantalla, Disco, Ratón, Impresora) y funciones auxiliares/menú.

  * **Función `main()`**:

      * Punto de entrada del programa.
      * Inicializa el modo de video (`pantalla_set_video_mode(0x03)`) y limpia la pantalla (`pantalla_clear_screen()`).
      * Contiene el bucle principal que muestra el menú y procesa la selección del usuario, llamando a las funciones de submenú correspondientes (`menu_pantalla_dos()`, `menu_disco_dos()`, etc.).

  * **Funciones Auxiliares**:

      * `print_regs_bios()`: Muestra los valores de los registros AX, BX, CX, DX para depuración.
      * `print_status_bios()`: Imprime si una operación fue exitosa o fallida, y el código de error del BIOS si aplica.
      * `press_enter_dos()`: Pausa la ejecución hasta que el usuario presione Enter.

  * **Módulos de Hardware (ej. `pantalla_set_video_mode`)**:

      * Cada función en estos módulos típicamente:
        1.  Muestra un mensaje informativo sobre la interrupción que se va a llamar.
        2.  Utiliza `print_regs_bios()` para mostrar los valores de los registros ANTES de la llamada a la interrupción.
        3.  Contiene un bloque `asm { ... }` donde se:
              * Mueven los parámetros necesarios a los registros del CPU (`ah`, `al`, `bx`, etc.).
              * Ejecuta la interrupción de software (ej. `int 0x10`).
              * Mueve los resultados de los registros (si los hay) a variables C++.
        4.  Utiliza `print_regs_bios()` para mostrar los valores de los registros DESPUÉS de la llamada.
        5.  Utiliza `print_status_bios()` para indicar el resultado.
      * El código ensamblador en línea (`asm`) permite el acceso directo a las instrucciones del CPU y a las variables C++ dentro del mismo bloque.

  * **Funciones de Menú (ej. `menu_pantalla_dos`)**:

      * Presentan las opciones del submenú.
      * Usan `cin` para obtener la entrada del usuario.
      * Llaman a las funciones del módulo de hardware correspondientes con los parámetros ingresados.
