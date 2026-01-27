/* * PROGRAMA: Compilador de Expresiones Booleanas
 * OBJETIVO: Generar código ensamblador para VM3 a partir de una expresión.
 * CARACTERÍSTICAS: Manejo de pila, gestión de variables y llamadas a IRQ.
 Por: Isaias Garcia Lopez
 Para: Arquitectura de la Computadora
 Profesor: Eduardo Virueña Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Estados del analizador
enum { FIN = 0, CONST, VAR, NOT, AND, OR, PI, PD, PYC, MAL };

char *buffer;
int pos = 0;
int token;
char lexema;
bool variables_usadas[26]; // Tabla de símbolos para rastrear variables a-z

// Marca qué variables aparecen en la expresión para reservar memoria después
void registrar_variable(char v) {
    if (v >= 'a' && v <= 'z') variables_usadas[v - 'a'] = true;
}

// Scanner: Identifica caracteres y variables
void tok() {
    while (buffer[pos] != '\0' && isspace(buffer[pos])) pos++;
    if (buffer[pos] == '\0') { token = FIN; return; }
    
    char c = buffer[pos++];
    lexema = c;
    
    if (c == '0' || c == '1') { token = CONST; return; }
    if (islower(c)) { token = VAR; registrar_variable(c); return; }
    
    switch (c) {
        case '!': token = NOT; break;
        case '&': token = AND; break;
        case '|': token = OR;  break;
        case '(': token = PI;  break;
        case ')': token = PD;  break;
        case ';': token = PYC; break;
        default:  token = MAL; break;
    }
}

void expr();

// TRADUCCIÓN DE PRIMARIOS: Genera carga de datos e inversión (NOT)
void primario() {
    if (token == CONST) {
        // Carga inmediata: lda #valor
        printf("\t lda #%d\n", lexema - '0');
        tok();
    } else if (token == VAR) {
        // Carga desde dirección de memoria: lda [@variable]
        printf("\t lda [@%c]\n", lexema); 
        tok();
    } else if (token == PI) {
        tok(); expr();
        if (token == PD) tok(); else exit(1);
    } else if (token == NOT) {
        tok(); primario();
        printf("\t not\n"); // Instrucción de VM3 para negar el acumulador
    }
}


// TRADUCCIÓN DE TÉRMINOS (AND): Usa la pila para operaciones binarias
void termino() {
    primario(); // Resultado del lado izquierdo queda en A
    while (token == AND) {
        tok();
        printf("\t psh a\n"); // Guardar operando izquierdo en la pila
        primario();           // Calcular lado derecho en A
        printf("\t and\n");   // La VM hace pop automático y opera con A
    }
}

// TRADUCCIÓN DE EXPRESIONES (OR)
void expr() {
    termino();
    while (token == OR) {
        tok();
        printf("\t psh a\n");
        termino();
        printf("\t or\n"); 
    }
}

int main() {
    char input[256];
    for(int i=0; i<26; i++) variables_usadas[i] = false;

    fprintf(stderr, "Ingrese expresion: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;

    // Escaneo rápido inicial para identificar variables y preparar @main
    for(int i=0; input[i] != '\0'; i++) {
        if(islower(input[i])) variables_usadas[input[i]-'a'] = true;
    }

    buffer = input;
    tok();

    // --- GENERACIÓN DE CÓDIGO ENSAMBLADOR ---
    
    // Punto de entrada de la VM (Vector 0x0000)
    printf("* = 0x0000\n");      
    printf("\t jmp @main\n");           

    // SECCIÓN DE LÓGICA (@linea)
    printf("\n* = 0x0100\n");    
    printf("@linea:\n");
    printf("\t psh bp\n");      // Guardar puntero de base (Stack Frame)
    printf("\t lbp sp\n");      // Establecer nuevo marco
    expr();                     // Generar instrucciones de la expresión booleana
    printf("\t lsp bp\n");      // Restaurar puntero de pila
    printf("\t pop bp\n");      // Restaurar puntero de base
    printf("\t rtn\n");         // Retorno de subrutina

    // SECCIÓN PRINCIPAL (@main)
    printf("\n@main:\n");
    printf("\t lsp #0xF000\n"); // Inicialización del Stack Pointer en zona segura
    printf("\t psh bp\n");
    printf("\t lbp sp\n");

    // Bloque de entrada de datos (petición de valores al usuario)
    for(int i=0; i<26; i++) {
        if(variables_usadas[i]) {
            printf("\t lda #'%c'\n", i + 'a'); // Cargar nombre de variable
            printf("\t irq 5\n");              // Imprimir carácter (pr_chr)
            printf("\t lda #63\n");            // Cargar '?'
            printf("\t irq 5\n");
            printf("\t irq 3\n");              // Leer entero desde teclado (rd_int)
            printf("\t sta [@%c]\n", i + 'a'); // Guardar valor leído en la variable
        }
    }

    // Ejecución y Salida
    printf("\t jsr @linea\n");    // Llamada a la evaluación lógica
    printf("\t psh a\n");          // Resguardar el resultado final de la expresión
    printf("\t lda #61\n");        // Cargar '=' (ASCII 61)
    printf("\t irq 5\n");          // Imprimir '='
    printf("\t pop a\n");          // Recuperar el resultado booleano
    printf("\t irq 1\n");          // Imprimir el resultado numérico (0 o 1)
    printf("\t lda #10\n");        // Cargar salto de línea
    printf("\t irq 5\n");
    printf("\t irq 0\n");          // Detener la Máquina Virtual (vm_hlt)

    // SECCIÓN DE DATOS: Reserva de memoria para variables detectadas
    printf("\n; --- Datos ---\n");
    for(int i=0; i<26; i++) {
        if(variables_usadas[i]) printf("@%c: word 0\n", i + 'a');
    }
    printf("end @main\n"); // Indica al ensamblador el punto de inicio

    return 0;
}
