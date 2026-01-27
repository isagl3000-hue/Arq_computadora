#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char nombre;
    int direccion;
} Variable;

Variable variables[26];
int var_count = 0;
int next_addr = 0x1000;  // Dirección para variables

const char *input;
int pos = 0;

char peek() { return input[pos]; }
char next() { return input[pos++]; }

bool match(char c) {
    if (peek() == c) { next(); return true; }
    return false;
}

int encontrar_variable(char nombre) {
    for (int i = 0; i < var_count; i++) {
        if (variables[i].nombre == nombre) {
            return variables[i].direccion;
        }
    }
    // Crear nueva variable
    variables[var_count].nombre = nombre;
    variables[var_count].direccion = next_addr;
    next_addr += 2;  // Cada variable ocupa 2 bytes
    return variables[var_count++].direccion;
}

void compilar_expresion();

void compilar_primario() {
    if (peek() == '0' || peek() == '1') {
        char c = next();
        // Cargar constante
        printf("    lda #%c\n", c);
        printf("    psh a\n");
    } else if (peek() >= 'a' && peek() <= 'z') {
        char c = next();
        int addr = encontrar_variable(c);
        // Cargar variable
        printf("    lda [0x%04x]\n", addr);
        printf("    psh a\n");
    } else if (match('(')) {
        compilar_expresion();
        match(')');
    } else if (match('!')) {
        compilar_primario();
        // Aplicar NOT
        printf("    pop a\n");
        printf("    not\n");
        printf("    psh a\n");
    }
}

void compilar_termino() {
    compilar_primario();
    
    while (peek() == '&') {
        next();
        compilar_primario();
        // Aplicar AND
        printf("    pop a\n");
        printf("    pop b\n");
        printf("    and\n");
        printf("    psh a\n");
    }
}

void compilar_expresion() {
    compilar_termino();
    
    while (peek() == '|') {
        next();
        compilar_termino();
        // Aplicar OR
        printf("    pop a\n");
        printf("    pop b\n");
        printf("    or\n");
        printf("    psh a\n");
    }
}

/*void compilar_linea(const char *expr) {
    input = expr;
    pos = 0;
    var_count = 0;
    next_addr = 0x1000;
    
    printf("; Compilando: %s\n", expr);
    printf("    lsp #0x8000\n");  // Inicializar pila
    
    compilar_expresion();
    
    // Guardar resultado
    printf("    pop a\n");
    printf("    sta [0x2000]\n");  // Dirección para resultado
    
    // Imprimir resultado
    printf("    lda [0x2000]\n");
    printf("    irq pr_int\n");  // Imprimir entero
    printf("    irq vm_hlt\n");  // Terminar
    
    // Definir variables
    printf("\n; Variables\n");
    for (int i = 0; i < var_count; i++) {
        printf("    org 0x%04x\n", variables[i].direccion);
        printf("    word 0\n");  // Inicializar en 0
    }
    
    // Espacio para resultado
    printf("    org 0x2000\n");
    printf("resultado: word 0\n");
}*/

void compilar_linea(const char *expr) {
    input = expr;
    pos = 0;
    var_count = 0;
    next_addr = 0x1000;
    
    printf("; Expresion: %s\n\n", expr);
    
    // Agregar etiqueta main
    printf("main:\n");
    printf("    lsp #0x8000\n\n");
    
    printf("    ; --- Evaluacion de expresion ---\n");
    compilar_expresion();
    
    printf("\n    ; --- Mostrar resultado ---\n");
    printf("    pop a\n");
    printf("    sta resultado\n");
    printf("    lda resultado\n");
    printf("    irq pr_int       ; mostrar resultado\n");
    printf("    lda #10\n");
    printf("    irq pr_chr       ; salto de linea\n");
    printf("    irq vm_hlt       ; terminar\n\n");
    
    printf("    ; --- Area de datos ---\n");
    
    // Variables sin usar org
    for (int i = 0; i < var_count; i++) {
        printf("var_%c:  word 0   ; variable '%c'\n", 
               variables[i].nombre, variables[i].nombre);
    }
    
    printf("resultado: word 0\n\n");
    
    printf("    end main, 0x8000\n");
}

int main() {
    char expresion[256];
    
    printf("Compilador de expresiones booleanas para VM\n");
    printf("===========================================\n\n");
    
    printf("Ingrese expresion booleana: ");
    fgets(expresion, sizeof(expresion), stdin);
    expresion[strcspn(expresion, "\n")] = 0;
    
    // Crear archivo ensamblador
    FILE *asm_file = fopen("expresion.asm", "w");
    if (!asm_file) {
        printf("Error al crear archivo\n");
        return 1;
    }
    
    // Redirigir salida al archivo
    freopen("expresion.asm", "w", stdout);
    
    printf("; Expresion booleana compilada\n");
    printf("; %s\n\n", expresion);
    
    compilar_linea(expresion);
    
    printf("\n    end main, 0x8000\n");
    
    fclose(stdout);
    printf("\nArchivo 'expresion.asm' creado.\n");
    
    return 0;
}
