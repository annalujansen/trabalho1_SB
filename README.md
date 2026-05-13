# Trabalho Prático 1 — Montador e Simulador

## Alunos
- Anna Luiza Novaes Jansen Silva - 232024572

## Sistema Operacional
- macOS (Apple Silicon M4 Pro)
> Tratado como Linux conforme especificação — compilado com GCC via Homebrew.

## Compilador
- g++-15 (Homebrew GCC 15.2.0)

---

## Como Compilar

Na raiz do projeto, execute:

```bash
make
```

Isso gera o executável `montador` na pasta raiz.

Para limpar os arquivos gerados:

```bash
make clean
```

---

## Como Rodar

O programa possui 3 modos de execução, determinados pela extensão do arquivo:

### Modo 1 — Pré-processamento (.asm → .pre)
```bash
./montador myfile.asm
```
Gera `myfile.pre` com o código normalizado:
- Comentários removidos
- Letras convertidas para maiúsculas
- Diretivas EQU e IF resolvidas
- Espaços e tabulações desnecessários removidos
- Rótulo seguido de ENTER unido à linha seguinte
- Seção DATA sempre por último

### Modo 2 — Montagem (.pre → .obj + .pen)
```bash
./montador myfile.pre
```
Gera dois arquivos:
- `myfile.obj` — código de máquina em uma linha, separado por espaços (algoritmo de duas passagens)
- `myfile.pen` — código gerado pelo algoritmo de passagem única, com pendências não resolvidas marcadas como `?ROTULO`

### Modo 3 — Simulação (.obj)
```bash
./montador myfile.obj
```
Simula a execução do programa:
- Saídas do comando `OUTPUT` são exibidas no terminal
- Entradas do comando `INPUT` são lidas pelo teclado

---

## Exemplo de Uso Completo

```bash
./montador tests/myfile.asm   # gera myfile.pre
./montador tests/myfile.pre   # gera myfile.obj e myfile.pen
./montador tests/myfile.obj   # simula o programa
```

---

## Conjunto de Instruções da Máquina Hipotética

| Mnemônico | Opcode | Tamanho | Ação |
|-----------|--------|---------|------|
| ADD       | 01     | 2       | ACC ← ACC + mem(OP) |
| SUB       | 02     | 2       | ACC ← ACC - mem(OP) |
| MULT      | 03     | 2       | ACC ← ACC × mem(OP) |
| DIV       | 04     | 2       | ACC ← ACC ÷ mem(OP) |
| JMP       | 05     | 2       | PC ← OP |
| JMPN      | 06     | 2       | Se ACC < 0 então PC ← OP |
| JMPP      | 07     | 2       | Se ACC > 0 então PC ← OP |
| JMPZ      | 08     | 2       | Se ACC = 0 então PC ← OP |
| COPY      | 09     | 3       | mem(OP2) ← mem(OP1) |
| LOAD      | 10     | 2       | ACC ← mem(OP) |
| STORE     | 11     | 2       | mem(OP) ← ACC |
| INPUT     | 12     | 2       | mem(OP) ← entrada |
| OUTPUT    | 13     | 2       | saída ← mem(OP) |
| STOP      | 14     | 1       | Suspende a execução |

### Diretivas
| Diretiva  | Ação |
|-----------|------|
| SPACE [N] | Reserva N palavras de memória (padrão: 1) |
| CONST V   | Aloca 1 palavra com valor V (aceita decimal, negativo e hexa 0x) |
| EQU V     | Substituição textual — não gera código |
| IF V      | Inclui linha seguinte se V ≠ 0 |

---

## Estrutura do Projeto

```
SB/
├── src/
│   ├── main.cpp          # roteador de modos (.asm/.pre/.obj)
│   ├── preprocessor.cpp  # pré-processamento: EQU, IF, normalização
│   ├── preprocessor.h
│   ├── assembler.cpp     # montagem: duas passagens + tabela de símbolos
│   ├── assembler.h
│   ├── simulator.cpp     # simulador: carregador + ciclo busca-executa
│   ├── simulator.h
│   └── utils.h           # funções compartilhadas: tokenize, toUpper, etc.
├── myfile.asm            # programa principal de teste
├── Makefile
└── README.md
```