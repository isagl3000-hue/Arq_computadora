; Expresion booleana compilada
; !0&(x|y);

; Compilando: !0&(x|y);
    lsp #0x8000
    lda #0
    psh a
    pop a
    not
    psh a
    lda [0x1000]
    psh a
    lda [0x1002]
    psh a
    pop a
    pop b
    or
    psh a
    pop a
    pop b
    and
    psh a
    pop a
    sta [0x2000]
    lda [0x2000]
    irq pr_int
    irq vm_hlt

; Variables
    org 0x1000
    word 0
    org 0x1002
    word 0
    org 0x2000
resultado: word 0

    end main, 0x8000
