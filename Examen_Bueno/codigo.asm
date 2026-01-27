* = 0x0000
	 jmp @main

* = 0x0100
@linea:
	 psh bp
	 lbp sp
	 lda [@a]
	 psh a
	 lda [@b]
	 and
	 lsp bp
	 pop bp
	 rtn

@main:
	 lsp #0xF000
	 psh bp
	 lbp sp
	 lda #'a'
	 irq 5
	 lda #63
	 irq 5
	 irq 3
	 sta [@a]
	 lda #'b'
	 irq 5
	 lda #63
	 irq 5
	 irq 3
	 sta [@b]
	 jsr @linea
	 psh a
	 lda #61
	 irq 5
	 pop a
	 irq 1
	 lda #10
	 irq 5
	 irq 0

; --- Datos ---
@a: word 0
@b: word 0
end @main
