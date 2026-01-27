PRIMERO
// Ejercicios específicos para VMs
1. Punteros y aritmética de punteros
2. Estructuras y uniones
3. Manipulación de bits
4. Gestión de memoria dinámica
5. Tablas de funciones (como inst[])

SEGUNDO
1. Modelo de von Neumann
2. Registros y su propósito
3. Ciclo fetch-decode-execute
4. Organización de memoria
5. Interrupciones y I/O

TERCERO
// Solo 3 instrucciones
typedef enum { HALT, PUSH, ADD } Opcode;

void vm_execute(unsigned char *code) {
    int pc = 0, sp = 0;
    int stack[256];
    
    while (1) {
        switch (code[pc++]) {
            case HALT: return;
            case PUSH: stack[sp++] = code[pc++]; break;
            case ADD:  stack[sp-2] += stack[sp-1]; sp--; break;
        }
    }
}
Paso 2 - Añadir registros y flags
Paso 3 - Implementar más instrucciones
Paso 4 - Añadir sistema de I/O
