/* ejercicio2.c - Validador de Sintaxis */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum { FIN = 0, CONST, VAR, NOT, AND, OR, PI, PD, PYC, MAL };

char *buffer;
int pos = 0;
int token;
char lexema;

void error(const char *mensaje) {
    fprintf(stderr, "ERROR SINTACTICO: %s en posicion %d\n", mensaje, pos);
    exit(EXIT_FAILURE);
}

void tok() {
    while (buffer[pos] != '\0' && isspace(buffer[pos])) pos++;
    if (buffer[pos] == '\0') { token = FIN; return; }

    char c = buffer[pos++];
    lexema = c;

    if (c == '0' || c == '1') { token = CONST; return; }
    if (islower(c))           { token = VAR; return; }

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

void primario() {
    if (token == CONST || token == VAR) {
        tok();
    } else if (token == PI) {
        tok();
        expr();
        if (token == PD) tok(); else error("Falta cerrar parentesis ')'");
    } else if (token == NOT) {
        tok();
        primario();
    } else {
        error("Se esperaba constante, variable, '(' o '!'");
    }
}

void termino() {
    primario();
    while (token == AND) {
        tok();
        primario();
    }
}

void expr() {
    termino();
    while (token == OR) {
        tok();
        termino();
    }
}

int main() {
    char input[256];
    printf("Validador. Ingrese expresion: ");
    if (fgets(input, sizeof(input), stdin)) {
        buffer = input;
        tok();
        expr();
        if (token == PYC) printf("SINTAXIS CORRECTA\n");
        else error("Falta ';' o hay caracteres extra");
    }
    return 0;
}
