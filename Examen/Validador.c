#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

const char *input;
int pos = 0;

char peek() { return input[pos]; }
char next() { return input[pos++]; }

bool match(char c) {
    if (peek() == c) { next(); return true; }
    return false;
}

bool constante() {
    char c = peek();
    if (c == '0' || c == '1') {
        next();
        return true;
    }
    return false;
}

bool variable() {
    char c = peek();
    if (c >= 'a' && c <= 'z') {
        next();
        return true;
    }
    return false;
}

bool expresion();

bool primario() {
    if (constante()) return true;
    if (variable()) return true;
    
    if (match('(')) {
        if (expresion() && match(')')) return true;
        return false;
    }
    
    if (match('!')) {
        return primario();
    }
    
    return false;
}

bool termino() {
    if (!primario()) return false;
    
    while (peek() == '&') {
        next();
        if (!primario()) return false;
    }
    
    return true;
}

bool expresion() {
    if (!termino()) return false;
    
    while (peek() == '|') {
        next();
        if (!termino()) return false;
    }
    
    return true;
}

bool linea() {
    pos = 0;
    if (!expresion()) return false;
    if (!match(';')) return false;
    return peek() == '\0';
}

int main() {
    char expresion[256];
    
    printf("Validador de expresiones booleanas\n");
    printf("Ejemplos validos: a & b; (a | b) & !c; !0 & (x | y);\n\n");
    
    while (1) {
        printf("Ingrese expresion (o 'salir'): ");
        fgets(expresion, sizeof(expresion), stdin);
        expresion[strcspn(expresion, "\n")] = 0;
        
        if (strcmp(expresion, "salir") == 0) break;
        
        input = expresion;
        printf("'%s' -> %s\n\n", expresion, 
               linea() ? "VÁLIDA" : "INVÁLIDA");
    }
    
    return 0;
}
