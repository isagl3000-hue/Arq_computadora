/* * PROGRAMA: Validador de Sintaxis Booleana
 * OBJETIVO: Verificar que una expresión cumpla con la gramática:
 * Expr -> Termino { | Termino }
 * Termino -> Primario { & Primario }
 * Primario -> const | var | ( Expr ) | ! Primario
 Por: Isaias Garcia Lopez
 Para: Arquitectura de la Computadora
 Profesor: Eduardo Virueña Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Definición de los componentes léxicos (Tokens)
enum { FIN = 0, CONST, VAR, NOT, AND, OR, PI, PD, PYC, MAL };

char *buffer;    // Almacena la cadena de entrada
int pos = 0;     // Índice para recorrer la cadena
int token;       // Almacena el token actual
char lexema;     // Carácter actual bajo análisis

// Función para reportar errores y detener el análisis
void error(const char *mensaje) {
    fprintf(stderr, "ERROR SINTACTICO: %s en posicion %d\n", mensaje, pos);
    exit(EXIT_FAILURE);
}

// ANALIZADOR LÉXICO (Scanner): Clasifica el siguiente carácter en un Token
void tok() {
    // Ignorar espacios en blanco
    while (buffer[pos] != '\0' && isspace(buffer[pos])) pos++;
    
    if (buffer[pos] == '\0') { token = FIN; return; }

    char c = buffer[pos++];
    lexema = c;

    // Clasificación de caracteres
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

// Prototipo de expresión para permitir recursividad
void expr();

// Analiza los elementos de mayor jerarquía (operatividad básica)
void primario() {
    if (token == CONST || token == VAR) {
        tok(); // Si es constante o variable, es válido, avanzar al siguiente
    } else if (token == PI) {
        tok(); // Consumir '('
        expr(); // Analizar el contenido interno como una nueva expresión
        if (token == PD) tok(); // Consumir ')'
        else error("Falta cerrar parentesis ')'");
    } else if (token == NOT) {
        tok(); // Consumir '!'
        primario(); // El NOT puede ser recursivo (ej. !!a)
    } else {
        error("Se esperaba constante, variable, '(' o '!'");
    }
}

// Analiza la operación AND (mayor precedencia que OR)
void termino() {
    primario();
    while (token == AND) {
        tok(); // Consumir '&'
        primario();
    }
}

// Analiza la operación OR (menor precedencia)
void expr() {
    termino();
    while (token == OR) {
        tok(); // Consumir '|'
        termino();
    }
}

int main() {
    char input[256];
    printf("Validador. Ingrese expresion: ");
    
    if (fgets(input, sizeof(input), stdin)) {
        buffer = input;
        tok(); // Obtener el primer token para iniciar
        expr(); // Iniciar el descenso recursivo
        
        // La expresión debe finalizar obligatoriamente con ';'
        if (token == PYC) printf("SINTAXIS CORRECTA\n");
        else error("Falta ';' o hay caracteres extra al final");
    }
    return 0;
}
