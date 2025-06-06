# Programación Híbrida en C++ y Ensamblador

Este proyecto desarrolla un programa híbrido que combina C++ y ensamblador para interactuar con hardware de bajo nivel en un entorno DOS simulado mediante DOSBox. Incluye un menú interactivo que permite al usuario controlar módulos como Pantalla, Disco, Ratón e Impresora utilizando interrupciones del sistema. Este `README.md` detalla la instalación de DOSBox, la codificación del programa, la creación de un disco virtual y una explicación completa del funcionamiento del código.

---

## **Requisitos Previos**
- Sistema operativo: Linux (instrucciones adaptables a Windows o macOS).
- Software: DOSBox, Turbo C++ (compilador para C++ y ensamblador en DOS).
- Conocimientos básicos de comandos en terminal y ensamblador x86.

---

## **Fase 1: Instalación y Configuración de DOSBox**

DOSBox es un emulador que permite ejecutar aplicaciones DOS en sistemas operativos modernos. A continuación, se describen los pasos para instalarlo y configurarlo.

### **Instalación de DOSBox**
1. **Actualiza el sistema (Linux)**:
   ```bash
   sudo apt update && sudo apt upgrade
   ```
2. **Instala DOSBox**:
   ```bash
   sudo apt-get install dosbox
   ```
3. **Verifica la instalación**:
   - Inicia DOSBox ejecutando:
     ```bash
     dosbox
     ```
   - Deberías ver la interfaz de línea de comandos de DOSBox (`Z:\>`).

### **Preparación del Entorno**
1. **Descarga Turbo C++ y los archivos del proyecto**:
   - Descarga el archivo ZIP que contiene Turbo C++ y los archivos fuente (por ejemplo, desde un repositorio o enlace proporcionado).
   - Descomprímelo en una carpeta accesible, como:
     ```bash
     /home/usuario/Documents/asm/MASM
     ```
     *(Sustituye "usuario" por tu nombre de usuario).*

2. **Configuración de DOSBox**:
   - Localiza el archivo de configuración de DOSBox:
     ```bash
     /home/usuario/.dosbox/dosbox-0.74-3.conf
     ```
   - Edítalo con un editor de texto (como `nano` o `subl`) y agrega las siguientes líneas al final para montar automáticamente el directorio y configurar el teclado:
     ```bash
     [autoexec]
     keyb la          # Configura el teclado en español latinoamericano
     mount c: /home/usuario/Documents/asm/MASM   # Monta el directorio como unidad C:
     c:               # Cambia a la unidad C:
     ```

3. **Prueba la Configuración**:
   - Inicia DOSBox nuevamente:
     ```bash
     dosbox
     ```
   - Verifica que el directorio esté montado correctamente al ver el prompt `C:\>`.

---

## **Fase 2: Codificación del Programa**

El programa se desarrolla en C++ con funciones en ensamblador integradas mediante Turbo C++. Aquí se explica cómo crear, compilar y ejecutar el código.

### **Estructura del Código**
- **Archivo principal**: `HYBDEMO.CPP`.
- **Módulos**: Pantalla (`INT 10h`), Disco (`INT 13h`), Ratón (`INT 33h`), Impresora (`INT 17h`).
- **Propósito**: Crear un menú interactivo que invoque funciones de ensamblador para interactuar con el hardware.

### **Pasos para Codificar**
1. **Navega al directorio de Turbo C++ en DOSBox**:
   - Una vez en el prompt `C:\>`, ejecuta:
     ```bash
     CD TC
     CD BIN
     ```
   - Esto te lleva al directorio donde está el compilador `TCC.EXE`.

2. **Crea o edita el archivo fuente**:
   - Puedes editar `HYBDEMO.CPP` desde tu sistema operativo host (Linux) con un editor como Sublime Text:
     ```bash
     subl /home/usuario/Documents/asm/MASM/HYBDEMO.CPP
     ```
   - Alternativamente, usa un editor dentro de DOSBox como `EDIT.COM`.

3. **Compila el programa**:
   - En el prompt de DOSBox (`C:\TC\BIN>`), compila el código con:
     ```bash
     TCC -ml -IC:\TC\INCLUDE -LC:\TC\LIB C:\HYBDEMO.CPP
     ```
     - `-ml`: Usa el modelo de memoria grande.
     - `-I` y `-L`: Especifican las rutas de las bibliotecas y encabezados de Turbo C++.

4. **Ejecuta el programa**:
   - Si la compilación es exitosa, genera `HYBDEMO.EXE`. Ejecútalo con:
     ```bash
     HYBDEMO.EXE
     ```
   - Deberías ver el menú interactivo en la pantalla.

---

## **Fase 3: Creación y Montaje de un Disco Virtual**

Para simular operaciones de disco (como leer/escribir sectores), crearemos un disquete virtual y lo montaremos en DOSBox.

### **Creación del Disco Virtual**
1. **Genera una imagen de disquete**:
   - En Linux, crea una imagen de 1.44 MB (tamaño estándar de disquete):
     ```bash
     dd if=/dev/zero of=/home/usuario/disco.img bs=512 count=2880
     ```
     - `bs=512`: Tamaño de bloque (512 bytes por sector).
     - `count=2880`: Total de sectores para 1.44 MB.

2. **Formatea la imagen como FAT12**:
   - Usa el comando:
     ```bash
     mkfs.vfat -F 12 /home/usuario/disco.img
     ```
   - Esto simula un disquete formateado compatible con DOS.

### **Montaje en DOSBox**
1. **Monta la imagen automáticamente**:
   - Edita nuevamente `dosbox-0.74-3.conf` y agrega en `[autoexec]`:
     ```bash
     imgmount a /home/usuario/disco.img -t floppy
     ```
2. **Monta manualmente (opcional)**:
   - Dentro de DOSBox, ejecuta:
     ```bash
     imgmount a /home/usuario/disco.img -t floppy
     ```
3. **Verifica el montaje**:
   - Cambia a la unidad `A:` con:
     ```bash
     A:
     ```
   - Usa `DIR` para listar el contenido (inicialmente vacío).

---

## **Fase 4: Explicación Detallada del Funcionamiento del Código**

El programa `HYBDEMO.CPP` es una aplicación híbrida que usa C++ para la lógica de alto nivel (menú y entrada del usuario) y ensamblador para operaciones de bajo nivel mediante interrupciones DOS. A continuación, se detalla su estructura y funcionamiento.

### **Estructura General**
- **Menú Principal**:
  - Implementado en C++.
  - Opciones: Pantalla, Disco, Ratón, Impresora, Salir.
  - Usa un bucle para capturar la entrada del usuario (teclas numéricas o letras).

- **Integración con Ensamblador**:
  - Las funciones en ensamblador se invocan desde C++ usando la directiva `asm {}` de Turbo C++ o mediante archivos `.ASM` enlazados.
  - Cada módulo utiliza interrupciones específicas para interactuar con el hardware.

### **Módulos y Funcionalidades**
1. **Módulo de Pantalla (Interrupción `INT 10h`)**:
   - **Propósito**: Controlar la pantalla (modo de video, cursor, texto).
   - **Funciones principales**:
     - Cambiar modo de video (ejemplo: 80x25 texto).
     - Posicionar el cursor.
     - Escribir texto con atributos (color, fondo).
   - **Ejemplo de código en ensamblador**:
     ```asm
     mov ah, 00h       ; Función: Establecer modo de video
     mov al, 03h       ; Modo: 80x25 texto
     int 10h           ; Llamada a la interrupción
     ```

2. **Módulo de Disco (Interrupción `INT 13h`)**:
   - **Propósito**: Leer y escribir sectores en el disquete virtual (`A:`).
   - **Funciones principales**:
     - Leer sectores.
     - Escribir datos en sectores específicos.
   - **Ejemplo de código en ensamblador**:
     ```asm
     mov ah, 02h       ; Función: Leer sectores
     mov al, 01h       ; Número de sectores a leer
     mov ch, 00h       ; Cilindro 0
     mov cl, 01h       ; Sector 1
     mov dh, 00h       ; Cabeza 0
     mov dl, 00h       ; Unidad A:
     int 13h           ; Llamada a la interrupción
     ```

3. **Módulo de Ratón (Interrupción `INT 33h`)**:
   - **Propósito**: Controlar el ratón en la interfaz DOS.
   - **Funciones principales**:
     - Mostrar/ocultar el cursor.
     - Obtener posición y estado de los botones.
   - **Ejemplo de código en ensamblador**:
     ```asm
     mov ax, 0001h     ; Función: Mostrar cursor
     int 33h           ; Llamada a la interrupción
     ```

4. **Módulo de Impresora (Interrupción `INT 17h`)**:
   - **Propósito**: Enviar datos a la impresora y verificar su estado.
   - **Funciones principales**:
     - Imprimir caracteres.
     - Consultar estado de la impresora.
   - **Ejemplo de código en ensamblador**:
     ```asm
     mov ah, 00h       ; Función: Imprimir carácter
     mov al, 'A'       ; Carácter a imprimir
     mov dx, 00h       ; Puerto LPT1
     int 17h           ; Llamada a la interrupción
     ```

### **Flujo del Programa**
1. **Inicio**:
   - C++ inicializa el programa y muestra el menú principal.
2. **Selección del usuario**:
   - El usuario ingresa una opción (ejemplo: "1" para Pantalla).
3. **Ejecución de funciones**:
   - C++ llama a la función correspondiente en ensamblador.
   - La interrupción interactúa con el hardware simulado en DOSBox.
4. **Regreso al Menú**:
   - Tras ejecutar la operación, el control retorna al menú principal.

### **Notas Técnicas**
- **Turbo C++**: Permite integrar ensamblador inline con `asm {}` o enlazar archivos `.ASM`.
- **Limitaciones de DOSBox**: Algunas interrupciones (como `INT 17h` para impresora) pueden no funcionar completamente sin hardware físico, pero se simulan para propósitos educativos.
- **Portabilidad**: El código es específico para DOS y requiere ajustes para otros entornos.

---

## **Conclusión**
Este proyecto demuestra cómo combinar C++ y ensamblador para controlar hardware mediante interrupciones en un entorno DOS simulado. Con las instrucciones proporcionadas, puedes instalar DOSBox, codificar el programa, crear un disco virtual y entender su funcionamiento. ¡Explora y modifica el código para ampliar sus capacidades!