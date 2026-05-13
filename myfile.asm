; Programa: encontra o maior entre dois numeros

MAIOR equ 1      ; flag para ativar debug
DEBUG equ 0      ; debug desativado

section text     ; inicio da secao de codigo
input a          ; le primeiro numero
input b          ; le segundo numero
load a           ; carrega a no acumulador
sub b            ; subtrai b
jmpn bmaior      ; se negativo, b eh maior
if MAIOR
output a         ; saida: a eh maior ou igual
jmp fim          ; pula para o fim
bmaior:
output b         ; saida: b eh maior
fim:
stop             ; encerra

section data     ; inicio da secao de dados
a:               ; rotulo sozinho na linha
space            ; reserva espaco para a
b:               ; rotulo sozinho na linha
space            ; reserva espaco para b