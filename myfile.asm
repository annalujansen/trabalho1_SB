; Programa: encontra o maior entre dois numeros
; programa com minúsculas, comentários, tabulações, espaços etc para mostrar o funcionamento do .pre


MaIoR    EQU     1
DEBUG equ 0

SECTION TEXT

    input     a        ; lendo A
INPUT	b                ; tabulação

load a
SuB     b

jmpn     bMaior

IF MAIOR
    output a      ; A maior

jmp fim

; label separado da instrução
bMaior:
    
    OUTPUT      b

fim:
    stop


SECTION DATA

a: ; comentario no label
    space

b:
	SPACE

temp:
    const 10

; teste do copy sem espaço
buffer1: space
buffer2: space

SECTION TEXT

copy buffer1,buffer2