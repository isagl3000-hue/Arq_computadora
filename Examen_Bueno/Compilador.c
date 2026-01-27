#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

enum { FIN = 0, CONST, VAR, NOT, AND, OR, PI, PD, PYC, MAL };

char *buffer;
int pos = 0;
int token;
char lexema;
bool variables_usadas[26];

void registrar_variable(char v) {
    if (v >= 'a' && v <= 'z') variables_usadas[v - 'a'] = true;
}

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

void primario() {
    if (token == CONST) {
        printf("\t lda #%d\n", lexema - '0'); // Convertir char '0'/'1' a int 0/1
        tok();
    } else if (token == VAR) {
        printf("\t lda [@%c]\n", lexema); 
        tok();
    } else if (token == PI) {
        tok(); expr();
        if (token == PD) tok(); else exit(1);
    } else if (token == NOT) {
        tok(); primario();
        printf("\t not\n");
    }
}

void termino() {
    primario();
    while (token == AND) {
        tok();
        printf("\t psh a\n");
        primario();
        printf("\t and\n"); 
    }
}

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

    for(int i=0; input[i] != '\0'; i++) {
        if(islower(input[i])) variables_usadas[input[i]-'a'] = true;
    }

    buffer = input;
    tok();

    // ESTRUCTURA COMPATIBLE CON ASM3 Y VM3
    printf("* = 0x0000\n");      
    printf("\t jmp @main\n");           

    printf("\n* = 0x0100\n");    
    printf("@linea:\n");
    printf("\t psh bp\n");
    printf("\t lbp sp\n");
    expr(); 
    printf("\t lsp bp\n");
    printf("\t pop bp\n");
    printf("\t rtn\n");

    printf("\n@main:\n");
    printf("\t lsp #0xF000\n"); 
    printf("\t psh bp\n");
    printf("\t lbp sp\n");

    for(int i=0; i<26; i++) {
        if(variables_usadas[i]) {
            printf("\t lda #'%c'\n", i + 'a');
            printf("\t irq 5\n");  // Mostrar letra
            printf("\t lda #63\n"); // Mostrar '?'
            printf("\t irq 5\n");
            printf("\t irq 3\n");  // Leer numero
            printf("\t sta [@%c]\n", i + 'a');
        }
    }

    printf("\t jsr @linea\n");
    printf("\t psh a\n");      // GUARDAR RESULTADO DE LA EXPRESION

    printf("\t lda #61\n");    // '='
    printf("\t irq 5\n");

    printf("\t pop a\n");       // RECUPERAR RESULTADO EN A
    printf("\t irq 1\n");      // IMPRIMIR RESULTADO (0 o 1)

    printf("\t lda #10\n");    // Salto de linea
    printf("\t irq 5\n");
    printf("\t irq 0\n");      // Halt

    printf("\n; --- Datos ---\n");
    for(int i=0; i<26; i++) {
        if(variables_usadas[i]) printf("@%c: word 0\n", i + 'a');
    }
    // Formato de fin que le gusta a asm3
    printf("end @main\n"); 

    return 0;
}
