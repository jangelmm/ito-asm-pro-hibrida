/*
PROGRAMACIÓN HIBRIDA ENTRE ASM Y C++

Martinez Mendoza Jesus Angel

Asignatura: Lenguaje de Interfaz
Semestre: 6to
Docente: Silva Martinez Dalia

Fecha: Jun - 2025
*/

#include <iostream.h> 
#include <iomanip.h>  
#include <conio.h>    
#include <dos.h>      

// --- Estructuras ---

struct Registers { 
    unsigned short ax, bx, cx, dx, si, di, ds, es, flags;
};

struct DiskStatus {
    char success_flag;        // 0 para falso (fallo), 1 para verdadero (exito)
    unsigned char error_code;
};

struct PrinterStatus {
    char success_flag;        // 0 para falso, 1 para verdadero
    unsigned char status_byte;
};

// --- Prototipos de Funciones de Modulo ---

// Modulo de Pantalla
void pantalla_set_video_mode(unsigned char mode);
void pantalla_set_cursor_type(unsigned char start_line, unsigned char end_line);
void pantalla_set_cursor_position(unsigned char row, unsigned char col, unsigned char page_num = 0);
void pantalla_get_cursor_position(unsigned char page_num, unsigned char* row, unsigned char* col, unsigned short* cursor_type);
void pantalla_clear_screen();
void pantalla_write_char_attr(char character, unsigned char attribute, unsigned char page_num, unsigned short times = 1);
void pantalla_scroll_window_up(unsigned char num_lines, unsigned char attribute, unsigned char r1, unsigned char c1, unsigned char r2, unsigned char c2);
void pantalla_change_video_page(unsigned char page_num);  // Nueva funcionalidad

// Modulo de Disco
DiskStatus disco_reset_disk_system(unsigned char drive);
DiskStatus disco_get_status(unsigned char drive);
DiskStatus disco_read_sectors(unsigned char drive, unsigned short cylinder, unsigned char head, unsigned char sector, unsigned char num_sectors, void far* buffer);
DiskStatus disco_write_sectors(unsigned char drive, unsigned short cylinder, unsigned char head, unsigned char sector, unsigned char num_sectors, void far* buffer);  // Nueva funcionalidad

// Modulo de Mouse
char mouse669_reset_and_status(unsigned short* num_buttons); // Devuelve char (0 o 1)
void mouse_show_cursor();
void mouse_hide_cursor();
void mouse_get_position_and_status(unsigned short* x, unsigned short* y, unsigned short* button_status);
void mouse_set_cursor_position(unsigned short x, unsigned short y);
void mouse_set_sensitivity(unsigned short horizontal, unsigned short vertical);  // Nueva funcionalidad

// Modulo de Impresora
PrinterStatus impresora_send_char(unsigned char character, unsigned char printer_num = 0);
PrinterStatus impresora_initialize(unsigned char printer_num = 0);
PrinterStatus impresora_get_status(unsigned char printer_num = 0);
PrinterStatus impresora_control(unsigned char control_code, unsigned char printer_num = 0);  // Nueva funcionalidad

// Modulo USB
void usb_placeholder_function();

// --- Prototipos de Funciones Auxiliares y de Menu ---
void print_regs_bios(const char* action, unsigned short ax, unsigned short bx, unsigned short cx, unsigned short dx);
void print_status_bios(const char* message, char success_flag = 1, unsigned char error_code = 0); // Usa char
void press_enter_dos();
void menu_pantalla_dos();
void menu_disco_dos();
void menu_raton_dos();
void menu_impresora_dos();

// Buffer global para lectura de disco
unsigned char far disk_read_buffer[512];

// --- FUNCION PRINCIPAL (main) ---
void main() {
    pantalla_set_video_mode(0x03);
    pantalla_clear_screen();

    int opcion_principal;
    char dummy_buffer_main[80];

    do {
        pantalla_clear_screen();
        cout << "SISTEMA DE CONTROL DE HARDWARE (DOSBox - INT Reales)\n";
        cout << "===================================================\n";
        cout << "1. PANTALLA (INT 10h)\n";
        cout << "2. DISCO (INT 13h)\n";
        cout << "3. RATON (INT 33h)\n";
        cout << "4. IMPRESORA (INT 17h)\n";
        cout << "5. SALIR\n";
        cout << "\nSeleccione una opcion: ";
        cin >> opcion_principal;
        // Limpieza de buffer simple para Turbo C++
        while(cin.get() != '\n');

        switch (opcion_principal) {
            case 1: menu_pantalla_dos(); break;
            case 2: menu_disco_dos(); break;
            case 3: menu_raton_dos(); break;
            case 4: menu_impresora_dos(); break;
            case 5: cout << "Saliendo del sistema...\n"; break;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
                press_enter_dos();
        }
    } while (opcion_principal != 5);

    cout << "Programa finalizado." << endl;
}

// --- Definiciones de Funciones Auxiliares ---
void print_regs_bios(const char* action, unsigned short ax_val, unsigned short bx_val, unsigned short cx_val, unsigned short dx_val) {
    cout << "--- REGISTROS para " << action << " ---\n";
    cout << " AX: 0x" << hex << setw(4) << setfill('0') << ax_val
         << " BX: 0x" << hex << setw(4) << setfill('0') << bx_val
         << " CX: 0x" << hex << setw(4) << setfill('0') << cx_val
         << " DX: 0x" << hex << setw(4) << setfill('0') << dx_val << dec << "\n";
}

void print_status_bios(const char* message, char success_flag, unsigned char error_code) { // Usa char
    cout << message;
    if (!success_flag) { // 0 es falso
        cout << " - FRACASO (BIOS Error: 0x" << hex << (int)error_code << dec << ")";
    } else { // no-cero es verdadero
        cout << " - EXITO (BIOS OK)";
    }
    cout << endl;
}

void press_enter_dos() {
    cout << "\nPresione Enter para continuar...";
    while (kbhit()) getch();
    getch();
    cout << endl;
}

// --- Definiciones de Implementaciones de Modulos (con INT reales) ---

// =============================================
// Modulo de Pantalla (Usando INT 10h Real)
// =============================================
void pantalla_set_video_mode(unsigned char param_mode) {
    cout << "PANTALLA: Llamando INT 10h AH=00h - Establecer Modo Video = 0x" << hex << (int)param_mode << dec << endl;
    print_regs_bios("INT 10h AH=00h (Entrada)", 0x0000 | param_mode, 0, 0, 0);
    asm {
        mov ah, 0x00
        mov al, param_mode // Usa el parametro directamente
        int 0x10
    }
    print_status_bios("INT 10h AH=00h ejecutada.");
}

void pantalla_set_cursor_position(unsigned char param_row, unsigned char param_col, unsigned char param_page_num) {
    cout << "PANTALLA: Llamando INT 10h AH=02h - Posicionar Cursor. Fila=" << (int)param_row << ", Col=" << (int)param_col << ", Pag=" << (int)param_page_num << endl;
    print_regs_bios("INT 10h AH=02h (Entrada)", 0x0200, (unsigned short)param_page_num << 8, 0, ((unsigned short)param_row << 8) | param_col);
    asm {
        mov ah, 0x02
        mov bh, param_page_num
        mov dh, param_row
        mov dl, param_col
        int 0x10
    }
    print_status_bios("INT 10h AH=02h ejecutada.");
}

void pantalla_get_cursor_position(unsigned char param_page_num, unsigned char* out_row, unsigned char* out_col, unsigned short* out_cursor_type) {
    cout << "PANTALLA: Llamando INT 10h AH=03h - Obtener Posicion Cursor. Pag=" << (int)param_page_num << endl;
    unsigned char temp_r, temp_c; // Variables locales para ASM
    unsigned short temp_ct;
    print_regs_bios("INT 10h AH=03h (Entrada)", 0x0300, (unsigned short)param_page_num << 8, 0, 0);
    asm {
        mov ah, 0x03
        mov bh, param_page_num
        int 0x10
        mov temp_r, dh
        mov temp_c, dl
        mov temp_ct, cx
    }
    *out_row = temp_r;
    *out_col = temp_c;
    if (out_cursor_type) *out_cursor_type = temp_ct;
    print_regs_bios("INT 10h AH=03h (Salida)", 0x0300, (unsigned short)param_page_num << 8, temp_ct, ((unsigned short)temp_r << 8) | temp_c);
    cout << "  -> Fila (DH): " << (int)temp_r << ", Col (DL): " << (int)temp_c << ", Tipo (CX): 0x" << hex << temp_ct << dec << endl;
    print_status_bios("INT 10h AH=03h ejecutada.");
}

void pantalla_write_char_attr(char param_char, unsigned char param_attr, unsigned char param_page, unsigned short param_times) {
    cout << "PANTALLA: Llamando INT 10h AH=09h - Escribir Car/Attr. Char='" << param_char << "', Attr=0x" << hex << (int)param_attr
              << dec << ", Veces=" << (int)param_times << ", Pag=" << (int)param_page << endl;
    print_regs_bios("INT 10h AH=09h (Entrada)", (0x0900 | (unsigned char)param_char), ((unsigned short)param_page << 8) | param_attr, param_times, 0);
    print_status_bios("INT 10h AH=09h ejecutada.");
    asm {
        mov ah, 0x09
        mov al, param_char
        mov bl, param_attr
        mov bh, param_page
        mov cx, param_times
        int 0x10
    }

    
}

void pantalla_clear_screen() {
    clrscr(); // Funcion de conio.h
    cout << "PANTALLA: Pantalla limpiada (usando clrscr de conio.h)." << endl;
    print_status_bios("Limpieza de pantalla ejecutada.");
}

void pantalla_set_cursor_type(unsigned char param_start, unsigned char param_end) {
    cout << "PANTALLA: Llamando INT 10h AH=01h - Establecer Tipo Cursor (start=" << (int)param_start << ", end=" << (int)param_end << ")" << endl;
    print_regs_bios("INT 10h AH=01h (Entrada)", 0x0100, 0, ((unsigned short)param_start << 8) | param_end, 0);
    asm {
        mov ah, 0x01
        mov ch, param_start
        mov cl, param_end
        int 0x10
    }
    print_status_bios("INT 10h AH=01h ejecutada.");
}

void pantalla_scroll_window_up(unsigned char param_lines, unsigned char param_attr, unsigned char param_r1, unsigned char param_c1, unsigned char param_r2, unsigned char param_c2) {
    cout << "PANTALLA: Llamando INT 10h AH=06h - Scroll Ventana Arriba (lines=" << (int)param_lines << ")" << endl;
    print_regs_bios("INT 10h AH=06h Scroll (Entrada)", (0x0600 | param_lines), (unsigned short)param_attr << 8, ((unsigned short)param_r1 << 8) | param_c1, ((unsigned short)param_r2 << 8) | param_c2);
    asm {
        mov ah, 0x06
        mov al, param_lines
        mov bh, param_attr
        mov ch, param_r1
        mov cl, param_c1
        mov dh, param_r2
        mov dl, param_c2
        int 0x10
    }
    print_status_bios("INT 10h AH=06h Scroll ejecutada.");
}

void pantalla_change_video_page(unsigned char page_num) {
    cout << "PANTALLA: Llamando INT 10h AH=05h - Cambiar Pagina de Video a " << (int)page_num << endl;
    print_regs_bios("INT 10h AH=05h (Entrada)", 0x0500 | page_num, 0, 0, 0);
    asm {
        mov ah, 0x05
        mov al, page_num
        int 0x10
    }
    print_status_bios("INT 10h AH=05h ejecutada.");
}

// =============================================
// Modulo de Disco (Usando INT 13h Real)
// =============================================
DiskStatus disco_reset_disk_system(unsigned char param_drive) {
    cout << "DISCO: Llamando INT 13h AH=00h - Resetear Drive=0x" << hex << (int)param_drive << dec << endl;
    DiskStatus status_ret;
    unsigned char ah_res;
    print_regs_bios("INT 13h AH=00h (Entrada)", 0x00, 0, 0, param_drive);
    asm {
        mov ah, 0x00
        mov dl, param_drive
        int 0x13
        mov ah_res, ah
    }
    status_ret.error_code = ah_res;
    status_ret.success_flag = (ah_res == 0) ? 1 : 0;
    print_regs_bios("INT 13h AH=00h (Salida)", (unsigned short)ah_res << 8, 0, 0, param_drive);
    print_status_bios("INT 13h AH=00h ejecutada.", status_ret.success_flag, status_ret.error_code);
    return status_ret;
}

DiskStatus disco_get_status(unsigned char param_drive) {
    cout << "DISCO: Llamando INT 13h AH=01h - Obtener Estado Drive=0x" << hex << (int)param_drive << dec << endl;
    DiskStatus status_ret;
    unsigned char ah_res;
    print_regs_bios("INT 13h AH=01h (Entrada)", 0x0100, 0, 0, param_drive);
    asm {
        mov ah, 0x01
        mov dl, param_drive
        int 0x13
        mov ah_res, ah
    }
    status_ret.error_code = ah_res;
    status_ret.success_flag = (ah_res == 0) ? 1 : 0;
    print_regs_bios("INT 13h AH=01h (Salida)", (unsigned short)ah_res << 8, 0, 0, param_drive);
    print_status_bios("INT 13h AH=01h ejecutada.", status_ret.success_flag, status_ret.error_code);
    return status_ret;
}

DiskStatus disco_read_sectors(unsigned char param_drive, unsigned short param_cyl, unsigned char param_head, unsigned char param_sect, unsigned char param_num_sect, void far* param_buffer) {
    cout << "DISCO: Llamando INT 13h AH=02h - Leer Sectores. Drive=0x" << hex << (int)param_drive
         << dec << " C=" << param_cyl << " H=" << (int)param_head << " S=" << (int)param_sect << " N=" << (int)param_num_sect << endl;
    DiskStatus status_ret;
    unsigned char ah_res;
    unsigned char al_read;
    unsigned short buf_seg = FP_SEG(param_buffer);
    unsigned short buf_off = FP_OFF(param_buffer);
    unsigned char val_ch = (unsigned char)(param_cyl & 0xFF);
    unsigned char val_cl = (unsigned char)((param_sect & 0x3F) | ((param_cyl >> 2) & 0xC0));

    print_regs_bios("INT 13h AH=02h (Entrada)", (0x0200 | param_num_sect), buf_off , (val_ch << 8) | val_cl , (param_head << 8) | param_drive );
    cout << "  Buffer ES (segmento): 0x" << hex << buf_seg << dec << endl;

    asm {
        push es
        mov ax, buf_seg
        mov es, ax
        mov bx, buf_off

        mov ah, 0x02
        mov al, param_num_sect
        mov ch, val_ch
        mov cl, val_cl
        mov dh, param_head
        mov dl, param_drive
        int 0x13

        mov ah_res, ah
        mov al_read, al
        pop es
    }
    status_ret.error_code = ah_res;
    status_ret.success_flag = (ah_res == 0) ? 1 : 0;
    print_regs_bios("INT 13h AH=02h (Salida)", ((unsigned short)ah_res << 8) | al_read, buf_off, (val_ch << 8) | val_cl, (param_head << 8) | param_drive);
    print_status_bios("INT 13h AH=02h ejecutada.", status_ret.success_flag, status_ret.error_code);
    if (status_ret.success_flag) {
        cout << "  Sectores leidos realmente: " << (int)al_read << endl;
    }
    return status_ret;
}

DiskStatus disco_write_sectors(unsigned char param_drive, unsigned short param_cyl, unsigned char param_head, unsigned char param_sect, unsigned char param_num_sect, void far* param_buffer) {
    cout << "DISCO: Llamando INT 13h AH=03h - Escribir Sectores. Drive=0x" << hex << (int)param_drive
         << dec << " C=" << param_cyl << " H=" << (int)param_head << " S=" << (int)param_sect << " N=" << (int)param_num_sect << endl;
    DiskStatus status_ret;
    unsigned char ah_res;
    unsigned char al_written;
    unsigned short buf_seg = FP_SEG(param_buffer);
    unsigned short buf_off = FP_OFF(param_buffer);
    unsigned char val_ch = (unsigned char)(param_cyl & 0xFF);
    unsigned char val_cl = (unsigned char)((param_sect & 0x3F) | ((param_cyl >> 2) & 0xC0));

    print_regs_bios("INT 13h AH=03h (Entrada)", (0x0300 | param_num_sect), buf_off, (val_ch << 8) | val_cl, (param_head << 8) | param_drive);
    cout << "  Buffer ES (segmento): 0x" << hex << buf_seg << dec << endl;

    asm {
        push es
        mov ax, buf_seg
        mov es, ax
        mov bx, buf_off

        mov ah, 0x03
        mov al, param_num_sect
        mov ch, val_ch
        mov cl, val_cl
        mov dh, param_head
        mov dl, param_drive
        int 0x13

        mov ah_res, ah
        mov al_written, al
        pop es
    }
    status_ret.error_code = ah_res;
    status_ret.success_flag = (ah_res == 0) ? 1 : 0;
    print_regs_bios("INT 13h AH=03h (Salida)", ((unsigned short)ah_res << 8) | al_written, buf_off, (val_ch << 8) | val_cl, (param_head << 8) | param_drive);
    print_status_bios("INT 13h AH=03h ejecutada.", status_ret.success_flag, status_ret.error_code);
    if (status_ret.success_flag) {
        cout << "  Sectores escritos realmente: " << (int)al_written << endl;
    }
    return status_ret;
}

// =============================================
// Modulo de Mouse (Usando INT 33h Real)
// =============================================
char mouse_reset_and_status(unsigned short* out_num_buttons) { // Devuelve char
    cout << "RATON: Llamando INT 33h AX=0000h - Reset/Status Mouse." << endl;
    unsigned short ax_res, bx_res;
    print_regs_bios("INT 33h AX=0000h (Entrada)", 0x0000, 0, 0, 0);
    asm {
        mov ax, 0x0000
        int 0x33
        mov ax_res, ax
        mov bx_res, bx
    }
    if (out_num_buttons) *out_num_buttons = bx_res;
    print_regs_bios("INT 33h AX=0000h (Salida)", ax_res, bx_res, 0, 0);
    char success = (ax_res == 0xFFFF) ? 1 : 0;
    print_status_bios("INT 33h AX=0000h ejecutada.", success, success ? 0 : 1);
    if(success) cout << "  Mouse detectado. Botones: " << bx_res << endl;
    else cout << "  Mouse NO detectado o driver MOUSE.COM no cargado." << endl;
    return success;
}

void mouse_show_cursor() {
    cout << "RATON: Llamando INT 33h AX=0001h - Mostrar Cursor Mouse." << endl;
    print_regs_bios("INT 33h AX=0001h (Entrada)", 0x0001, 0, 0, 0);
    asm { mov ax, 0x0001; int 0x33; }
    print_status_bios("INT 33h AX=0001h ejecutada.");
}

void mouse_hide_cursor() {
    cout << "RATON: Llamando INT 33h AX=0002h - Ocultar Cursor Mouse." << endl;
    print_regs_bios("INT 33h AX=0002h (Entrada)", 0x0002, 0, 0, 0);
    asm { mov ax, 0x0002; int 0x33; }
    print_status_bios("INT 33h AX=0002h ejecutada.");
}

void mouse_get_position_and_status(unsigned short* out_x, unsigned short* out_y, unsigned short* out_button_status) {
    cout << "RATON: Llamando INT 33h AX=0003h - Obtener Posicion/Botones Mouse." << endl;
    unsigned short bx_res, cx_res, dx_res;
    print_regs_bios("INT 33h AX=0003h (Entrada)", 0x0003, 0, 0, 0);
    asm {
        mov ax, 0x0003
        int 0x33
        mov bx_res, bx
        mov cx_res, cx
        mov dx_res, dx
    }
    if (out_x) *out_x = cx_res;
    if (out_y) *out_y = dx_res;
    if (out_button_status) *out_button_status = bx_res;
    print_regs_bios("INT 33h AX=0003h (Salida)", 0x0003, bx_res, cx_res, dx_res);
    cout << "  Posicion: X=" << cx_res << ", Y=" << dx_res << ". Botones (BX)=0x" << hex << bx_res << dec << endl;
    print_status_bios("INT 33h AX=0003h ejecutada.");
}

void mouse_set_cursor_position(unsigned short param_x, unsigned short param_y) {
    cout << "RATON: Llamando INT 33h AX=0004h - Establecer Posicion Mouse. X="<< param_x << ", Y=" << param_y << endl;
    print_regs_bios("INT 33h AX=0004h (Entrada)", 0x0004, 0, param_x, param_y);
    asm {
        mov ax, 0x0004
        mov cx, param_x
        mov dx, param_y
        int 0x33
    }
    print_status_bios("INT 33h AX=0004h ejecutada.");
}

void mouse_set_sensitivity(unsigned short horizontal, unsigned short vertical) {
    cout << "RATON: Llamando INT 33h AX=001Ah - Configurar Sensibilidad Horizontal=" << horizontal << ", Vertical=" << vertical << endl;
    print_regs_bios("INT 33h AX=001Ah (Entrada)", 0x001A, horizontal, vertical, 0);
    asm {
        mov ax, 0x001A
        mov bx, horizontal
        mov cx, vertical
        int 0x33
    }
    print_status_bios("INT 33h AX=001Ah ejecutada.");
}

// =============================================
// Modulo de Impresora (Usando INT 17h Real)
// =============================================
PrinterStatus impresora_send_char(unsigned char character, unsigned char printer_num) {
    cout << "IMPRESORA: Llamando INT 17h AH=00h - Enviar Char '" << character << "' a LPT" << (int)printer_num + 1 << "." << endl;
    PrinterStatus status_ret;
    unsigned char ah_res;
    unsigned short dx_param = printer_num;
    print_regs_bios("INT 17h AH=00h (Entrada)", (0x0000 | character), 0, 0, dx_param);
    asm {
        mov ah, 0x00
        mov al, character
        mov dx, dx_param
        int 0x17
        mov ah_res, ah
    }
    status_ret.status_byte = ah_res;
    status_ret.success_flag = !(ah_res & 0x08) ? 1 : 0;
    print_regs_bios("INT 17h AH=00h (Salida)", ((unsigned short)ah_res << 8) | character, 0, 0, dx_param);
    print_status_bios("INT 17h AH=00h ejecutada.", status_ret.success_flag, status_ret.success_flag ? 0 : ah_res);
    return status_ret;
}

PrinterStatus impresora_initialize(unsigned char printer_num) {
    cout << "IMPRESORA: Llamando INT 17h AH=01h - Inicializar LPT" << (int)printer_num + 1 << "." << endl;
    PrinterStatus status_ret;
    unsigned char ah_res;
    unsigned short dx_param = printer_num;
    print_regs_bios("INT 17h AH=01h (Entrada)", 0x0100, 0, 0, dx_param);
    asm {
        mov ah, 0x01
        mov dx, dx_param
        int 0x17
        mov ah_res, ah
    }
    status_ret.status_byte = ah_res;
    status_ret.success_flag = !(ah_res & 0x08) ? 1 : 0;
    print_regs_bios("INT 17h AH=01h (Salida)", (unsigned short)ah_res << 8, 0, 0, dx_param);
    print_status_bios("INT 17h AH=01h ejecutada.", status_ret.success_flag, status_ret.success_flag ? 0 : ah_res);
    return status_ret;
}

PrinterStatus impresora_get_status(unsigned char printer_num) {
    cout << "IMPRESORA: Llamando INT 17h AH=02h - Obtener Estado LPT" << (int)printer_num + 1 << "." << endl;
    PrinterStatus status_ret;
    unsigned char ah_res;
    unsigned short dx_param = printer_num;
    print_regs_bios("INT 17h AH=02h (Entrada)", 0x0200, 0, 0, dx_param);
    asm {
        mov ah, 0x02
        mov dx, dx_param
        int 0x17
        mov ah_res, ah
    }
    status_ret.status_byte = ah_res;
    status_ret.success_flag = 1; // Obtener estado siempre "funciona"
    print_regs_bios("INT 17h AH=02h (Salida)", (unsigned short)ah_res << 8, 0, 0, dx_param);
    cout << "  Estado crudo (AH): 0x" << hex << (int)ah_res << dec << endl;
    if (ah_res & 0x80) cout << "    Impresora NO Ocupada" << endl; else cout << "    Impresora Ocupada" << endl;
    print_status_bios("INT 17h AH=02h ejecutada.", status_ret.success_flag, 0);
    return status_ret;
}

PrinterStatus impresora_control(unsigned char control_code, unsigned char printer_num) {
    cout << "IMPRESORA: Llamando INT 17h AH=00h - Enviar Codigo de Control 0x" << hex << (int)control_code << dec << " a LPT" << (int)printer_num + 1 << "." << endl;
    PrinterStatus status_ret;
    unsigned char ah_res;
    unsigned short dx_param = printer_num;
    print_regs_bios("INT 17h AH=00h (Entrada)", (0x0000 | control_code), 0, 0, dx_param);
    asm {
        mov ah, 0x00
        mov al, control_code
        mov dx, dx_param
        int 0x17
        mov ah_res, ah
    }
    status_ret.status_byte = ah_res;
    status_ret.success_flag = !(ah_res & 0x08) ? 1 : 0;
    print_regs_bios("INT 17h AH=00h (Salida)", ((unsigned short)ah_res << 8) | control_code, 0, 0, dx_param);
    print_status_bios("INT 17h AH=00h ejecutada.", status_ret.success_flag, status_ret.success_flag ? 0 : ah_res);
    return status_ret;
}

// Funcion usb_placeholder_function
void usb_placeholder_function() {
    cout << "Modulo USB (Placeholder) - No implementado con BIOS/DOS estandar." << endl;
    asm { nop; }
    print_status_bios("Funcion usb_placeholder_function ejecutada.");
}

// --- Definiciones de Funciones de Menu ---

void menu_pantalla_dos() {
    int opcion_menu;
    unsigned char r_t=0, c_t=0, page_t=0, mode_t=0, attr_t=0;
    unsigned short cursor_t_val=0, times_t=0;
    char char_t=' ';
    unsigned int temp_input_val;
    char dummy_buf[80];

    do {
        clrscr();
        cout << "\n--- MODULO PANTALLA (DOS - INT 10h) ---\n";
        cout << "1. Establecer Modo Video (AH=00h)\n";
        cout << "2. Establecer Tipo Cursor (AH=01h)\n";
        cout << "3. Posicionar Cursor (AH=02h)\n";
        cout << "4. Obtener Posicion Cursor (AH=03h)\n";
        cout << "5. Escribir Caracter/Atributo (AH=09h)\n";
        cout << "6. Limpiar Pantalla (con clrscr)\n";
        cout << "7. Scroll Ventana Arriba (AH=06h)\n";
        cout << "8. Cambiar Pagina de Video (AH=05h)\n";  // Nueva opción
        cout << "0. Volver al Menu Principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion_menu;
        while(cin.get()!='\n'); // Limpiar buffer simple

        switch (opcion_menu) {
            case 1:
                cout << "Modo de video (hex): ";
                cin >> hex >> temp_input_val;
                mode_t = (unsigned char)temp_input_val;
                while(cin.get()!='\n');
                pantalla_set_video_mode(mode_t);
                break;
            case 2:
                cout << "Linea inicio cursor (scanline): ";
                cin >> dec >> temp_input_val;
                r_t = (unsigned char)temp_input_val;
                cout << "Linea fin cursor (scanline): ";
                cin >> temp_input_val;
                c_t = (unsigned char)temp_input_val;
                while(cin.get()!='\n');
                pantalla_set_cursor_type(r_t, c_t);
                break;
            case 3:
                cout << "Fila: ";
                cin >> dec >> temp_input_val;
                r_t = (unsigned char)temp_input_val;
                cout << "Columna: ";
                cin >> temp_input_val;
                c_t = (unsigned char)temp_input_val;
                while(cin.get()!='\n');
                pantalla_set_cursor_position(r_t, c_t, 0);
                break;
            case 4:
                pantalla_get_cursor_position(0, &r_t, &c_t, &cursor_t_val);
                break;
            case 5:
                cout << "Caracter: ";
                cin >> char_t;
                cout << "Atributo (hex): ";
                cin >> hex >> temp_input_val;
                attr_t = (unsigned char)temp_input_val;
                cout << "Veces (dec): ";
                cin >> dec >> times_t;
                while(cin.get()!='\n');
                pantalla_write_char_attr(char_t, attr_t, 0, times_t);
                break;
            case 6:
                pantalla_clear_screen();
                break;
            case 7:
                cout << "Lineas a scrollear (ej.1): ";
                cin >> dec >> temp_input_val;
                r_t = (unsigned char)temp_input_val;
                cout << "Atributo para lineas nuevas (hex, ej.07): ";
                cin >> hex >> temp_input_val;
                attr_t = (unsigned char)temp_input_val;
                while(cin.get()!='\n');
                pantalla_scroll_window_up(r_t, attr_t, 0,0,24,79);
                break;
            case 8:
                cout << "Numero de Pagina de video (0-7): ";
                cin >> temp_input_val;
                page_t = (unsigned char)temp_input_val;
                while(cin.get()!='\n');
                pantalla_change_video_page(page_t);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
                break;
        }
        if (opcion_menu != 0) press_enter_dos();
    } while (opcion_menu != 0);
}

void menu_disco_dos() {
    int opcion_menu;
    unsigned char drive_param = 0x00;
    unsigned int temp_in;
    char dummy_b[80];

    do {
        clrscr();
        cout << "\n--- MODULO DISCO (DOS - INT 13h) ---\n";
        cout << "ADVERTENCIA: Operaciones de disco pueden ser peligrosas!\n";
        cout << "1. Resetear Sistema de Disco (AH=00h)\n";
        cout << "2. Obtener Estado de Disco (AH=01h)\n";
        cout << "3. Leer Sector de Prueba (AH=02h) del drive actual\n";
        cout << "4. Escribir Sector de Prueba (AH=03h) del drive actual\n";  // Nueva opción
        cout << "0. Volver al Menu Principal\n";
        cout << "Drive actual para pruebas: 0x" << hex << (int)drive_param << dec << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion_menu;
        while(cin.get()!='\n');

        switch(opcion_menu) {
            case 1:
                cout << "Drive a resetear (hex, 00=A:, 80=C:, actual=0x" << hex << (int)drive_param << dec << "): ";
                cin >> hex >> temp_in;
                drive_param = (unsigned char)temp_in;
                while(cin.get()!='\n');
                disco_reset_disk_system(drive_param);
                break;
            case 2:
                cout << "Drive para obtener estado (hex, actual=0x" << hex << (int)drive_param << dec << "): ";
                cin >> hex >> temp_in;
                drive_param = (unsigned char)temp_in;
                while(cin.get()!='\n');
                disco_get_status(drive_param);
                break;
            case 3:
                cout << "Leyendo Sector 1, C=0, H=0 de Drive 0x" << hex << (int)drive_param << dec << " al buffer global." << endl;
                disco_read_sectors(drive_param, 0, 0, 1, 1, disk_read_buffer);
                cout << "  Primeros 16 bytes del buffer leido (hex):" << endl << "  ";
                for(int i=0; i<16; i++) {
                    cout << hex << setw(2) << setfill('0') << (int)disk_read_buffer[i] << " ";
                }
                cout << dec << endl;
                break;
            case 4:
                cout << "Escribiendo Sector 1, C=0, H=0 de Drive 0x" << hex << (int)drive_param << dec << " desde buffer global." << endl;
                disco_write_sectors(drive_param, 0, 0, 1, 1, disk_read_buffer);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
                break;
        }
        if (opcion_menu != 0) press_enter_dos();
    } while (opcion_menu != 0);
}

void menu_raton_dos() {
    int opcion_menu;
    unsigned short num_botones_m=0, x_m_val=0, y_m_val=0, stat_m_val=0;
    char dummy_b_m[80];

    if (!mouse_reset_and_status(&num_botones_m)) {
        cout << "Driver de Mouse no detectado (INT 33h no disponible)." << endl;
        cout << "Asegurate de cargar MOUSE.COM o similar en DOS/DOSBox." << endl;
        press_enter_dos();
        return;
    }
    cout << "Mouse detectado con " << num_botones_m << " botones." << endl;
    press_enter_dos();

    do {
        clrscr();
        cout << "\n--- MODULO RATON (DOS - INT 33h) ---\n";
        cout << "1. Mostrar Cursor Mouse (AX=0001h)\n";
        cout << "2. Ocultar Cursor Mouse (AX=0002h)\n";
        cout << "3. Obtener Posicion/Botones (AX=0003h)\n";
        cout << "4. Establecer Posicion Cursor (AX=0004h)\n";
        cout << "5. Configurar Sensibilidad (AX=001Ah)\n";  // Nueva opción
        cout << "0. Volver al Menu Principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion_menu;
        while(cin.get()!='\n');

        switch(opcion_menu) {
            case 1:
                mouse_show_cursor();
                break;
            case 2:
                mouse_hide_cursor();
                break;
            case 3:
                mouse_get_position_and_status(&x_m_val, &y_m_val, &stat_m_val);
                cout << "  En C++: X=" << x_m_val << " Y=" << y_m_val << " Botones=0x" << hex << stat_m_val << dec << endl;
                break;
            case 4:
                cout << "Nueva X (0-639): ";
                cin >> x_m_val;
                cout << "Nueva Y (0-199 o 0-349 para texto): ";
                cin >> y_m_val;
                while(cin.get()!='\n');
                mouse_set_cursor_position(x_m_val, y_m_val);
                break;
            case 5:
                cout << "Sensibilidad Horizontal (0-100): ";
                cin >> x_m_val;
                cout << "Sensibilidad Vertical (0-100): ";
                cin >> y_m_val;
                while(cin.get()!='\n');
                mouse_set_sensitivity(x_m_val, y_m_val);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
                break;
        }
        if (opcion_menu != 0) press_enter_dos();
    } while (opcion_menu != 0);
}

void menu_impresora_dos() {
    int opcion_menu;
    unsigned char prn_p_val = 0;
    char char_p_val;
    unsigned int temp_in_p;
    char dummy_b_p[80];

    do {
        clrscr();
        cout << "\n--- MODULO IMPRESORA (DOS - INT 17h) ---\n";
        cout << "1. Inicializar Impresora (AH=01h)\n";
        cout << "2. Obtener Estado Impresora (AH=02h)\n";
        cout << "3. Enviar Caracter a Impresora (AH=00h)\n";
        cout << "4. Enviar Codigo de Control (AH=00h)\n";  // Nueva opción
        cout << "0. Volver al Menu Principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion_menu;
        while(cin.get()!='\n');

        switch(opcion_menu) {
            case 1:
                cout << "Numero impresora (0=LPT1, 1=LPT2, 2=LPT3): ";
                cin >> temp_in_p;
                if (temp_in_p <=2) prn_p_val = (unsigned char)temp_in_p;
                else cout << "Num invalido\n";
                while(cin.get()!='\n');
                impresora_initialize(prn_p_val);
                break;
            case 2:
                cout << "Numero impresora (0-2): ";
                cin >> temp_in_p;
                if (temp_in_p <=2) prn_p_val = (unsigned char)temp_in_p;
                else cout << "Num invalido\n";
                while(cin.get()!='\n');
                impresora_get_status(prn_p_val);
                break;
            case 3:
                cout << "Numero impresora (0-2): ";
                cin >> temp_in_p;
                if (temp_in_p <=2) prn_p_val = (unsigned char)temp_in_p;
                else cout << "Num invalido\n";
                cout << "Caracter a enviar: ";
                cin >> char_p_val;
                while(cin.get()!='\n');
                impresora_send_char(char_p_val, prn_p_val);
                break;
            case 4:
                cout << "Numero impresora (0-2): ";
                cin >> temp_in_p;
                if (temp_in_p <=2) prn_p_val = (unsigned char)temp_in_p;
                else cout << "Num invalido\n";
                cout << "Codigo de control (hex): ";
                cin >> hex >> temp_in_p;
                char_p_val = (unsigned char)temp_in_p;
                while(cin.get()!='\n');
                impresora_control(char_p_val, prn_p_val);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
                break;
        }
        if (opcion_menu != 0) press_enter_dos();
    } while (opcion_menu != 0);
}