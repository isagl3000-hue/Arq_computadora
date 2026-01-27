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
int next_addr = 0x0100;  // Cambié a 0x0100 para mejor organización

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
    variables[var_count].nombre = nombre;
    variables[var_count].direccion = next_addr;
    next_addr += 2;
    return variables[var_count++].direccion;
}

void compilar_expresion();

void compilar_primario() {
    if (peek() == '0' || peek() == '1') {
        char c = next();
        printf("    lda #%c\n", c);
        printf("    psh a\n");
    } else if (peek() >= 'a' && peek() <= 'z') {
        char c = next();
        int addr = encontrar_variable(c);
        printf("    lda var_%c   ; variable %c\n", c, c);
        printf("    psh a\n");
    } else if (match('(')) {
        compilar_expresion();
        match(')');
    } else if (match('!')) {
        compilar_primario();
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
        printf("    pop a\n");
        printf("    pop b\n");
        printf("    or\n");
        printf("    psh a\n");
    }
}

int main(int argc, char *argv[]) {
    char expresion[256];
    
    if (argc == 2) {
        // Modo: ./Compilador "expresion;"
        strcpy(expresion, argv[1]);
    } else {
        printf("Compilador de expresiones booleanas para VM\n");
        printf("===========================================\n\n");
        printf("Ingrese expresion booleana: ");
        fgets(expresion, sizeof(expresion), stdin);
        expresion[strcspn(expresion, "\n")] = 0;
    }
    
    // Verificar que termine con ;
    if (expresion[strlen(expresion)-1] != ';') {
        printf("Error: La expresion debe terminar con ';'\n");
        return 1;
    }
    
    input = expresion;
    pos = 0;
    
    printf("; Expresion: %s\n\n", expresion);
    printf("main:\n");
    printf("    lsp #0x8000\n\n");
    
    printf("    ; Codigo generado:\n");
    compilar_expresion();
    
    printf("\n    ; Mostrar resultado\n");
    printf("    pop a\n");
    printf("    sta resultado\n");
    printf("    lda resultado\n");
    printf("    irq pr_int\n");
    printf("    lda #10          ; salto de linea\n");
    printf("    irq pr_chr\n");
    printf("    irq vm_hlt\n\n");
    
    printf("    ; Variables\n");
    for (int i = 0; i < var_count; i++) {
        printf("var_%c:  word 0   ; variable '%c'\n", 
               variables[i].nombre, variables[i].nombre);
    }
    printf("resultado: word 0\n\n");
    
    printf("    end main, 0x8000\n");
    
    return 0;
}
