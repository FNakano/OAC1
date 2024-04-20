# Especificação

## Visão geral

Este é um (simulador de) processador de 16 bits e sua memória principal de 4k palavras (8kBytes). O arquivo-fonte do simulador é https://github.com/FNakano/OAC1/blob/main/meuProcessador/CarregaArmazenaSaltaArit4.circ . O arquivo-fonte deve ser executado no [logisim](http://www.cburch.com/logisim/). Um programa para demonstração, usado no desenvolvimento do processador é https://github.com/FNakano/OAC1/blob/main/meuProcessador/comHalt.mem . O passo a passo do desenvolvimento e a explicação do exemplo estão em https://github.com/FNakano/OAC1/tree/main/meuProcessador . 

A comunicação entre processador e memória é feita por um barramento de endereços de 12 bits, barramento de dados de 16 bits e sinais de controle.

O processador tem suporte para chamada de funções e retorno. 

Interrupções ainda não foram implementadas.

## Organização

O processador tem 7 registradores dos quais 4, *{A, B, C, D}* são de uso geral, um é o contador de programa (*PC*), um armazena endereços de retorno (*R*), um armazena a palavra de estado da execução de operações aritméticas (*PSW*).

A notação dos números é (*big endian*) ié,  MSB primeiro.

## Conjunto de Instruções


| OpCode  | Mnemônico | Descrição |
| ------  | --------- | --------- |
| (0000)b | NOP       | Não operação |
| (0001)b | LDA(X)    | Carrega conteúdo do acumulador com conteúdo armazenado no endereço X da memória |
| (0010)b | STA(X)    | Armazena no endereço X da memória o conteúdo do acumulador |
| (0011)b | JMP X     | Armazena o próximo endereço sequencial no registrador R e **salta** execução para instrução armazenada no endereço X da memória |
| (0100)b | JNZ X     | Se o acumulador for diferente de zero, armazena o próximo endereço sequencial no registrador R e salta execução para instrução armazenada no endereço X da memória |
| (0101)b | RET       | Armazena o próximo endereço sequencial no registrador R e **salta** execução para endereço armazenado no registrador R |
| (0110)b | ARIT     | Executa operação aritmética a atualiza registrador PSW |
| (1111)b | HALT     | Para o processador ("desabilita o clock"). Sua necessidade (didática) tornou-se clara para encerrar o simulador do EP1. CarregaArmazenaSaltaArit4.circ tem o circuito que implementa HALT, CarregaArmazenaSaltaArit3.circ não tem o circuito. |

Formato das instruções LDA, STA, JMP, JNZ, HALT:

![](./Captura%20de%20tela%20de%202024-04-17%2018-58-32.png)

Em uma palavra de memória, o código de operação (Op) ocupa os quatro bits mais significativos e o endereço de memória (X) ocupa os 12 bits restantes.

Na operação LDA o conteúdo da memória no endereço X é carregado no acumulador (registrador A).

Na operação STA o conteúdo do acumulador (registrador A) é armazenado no endereço X da memória.

Nas instruções JMP e JNZ, X é o endereço da memória onde está armazenada a próxima instrução caso o salto seja executado. Caso o salto não seja executado a próxima instrução é a do endereço seguinte do endereço onde está armazenada a instrução de salto. Ainda caso o salto seja executado, o endereço seguinte do endereço da instrução de salto é armazenado no registrador R.

Exemplos:

(1120)h = LDA(120) - carrega o acumulador com o valor contido no endereço 0x120;

(2151)h = STA(151) - armazena o valor contido no acumulador no conteúdo do endereço 0x120;

(3010)h = JMP 010 - salta para o endereço 0x010, ié passa a executar a partir da instrução armazenada no endereço 0x010;

(4010)h = JNZ 010 - Se o registrador A contiver valor diferente de zero, salta para o endereço 0x010, ié passa a executar a partir da instrução armazenada no endereço 0x010, caso contrário segue executando a instrução no endereço seguinte ao da instrução de salto;
 

![](./Captura%20de%20tela%20de%202024-04-17%2019-32-08.png)

Nas operações aritméticas o OpCode é (0110)b e o restante dos bits designa:
	
*Arit* - três bits - especifica a operação aritmética. A partir de zero: {Põe como resultado (FFFF)h, Põe como resultado (0000)h, not(Op1), and(Op1, Op2), or(Op1, Op2), xor(Op1, Op2), add(Op1, Op2}, sub(Op1, Op2)}

*Res* - registrador em que o resultado da operação aritmética deve ser armazenado. A partir de zero: {A, B, C, D, x, x, R, X}. **nota**: R não foi testado e PSW não está conectado para receber resultados.

*Op1* - registrador que contém o primeiro operando. A partir de zero: {A, B, C, D, x, x, R, X}.

*Op2* - caso o bit mais significativo seja zero, assume-se que o segundo operando valha zero, caso o bit seja 1, os dois bits seguintes designam um dos registradores:  A partir de zero: {A, B, C, D}.

Exemplos:

// carga do acumulador para um registrador - verificado em 2024-04-11, funcionam

(6C40)h - 0110 110 001 000 0 00 - ARIT ADD, B, A, zero - armazena no registrador B o resultado de A + 0

(6C80)h - 0110 110 010 000 0 00 - ARIT ADD, C, A, zero -  armazena no registrador C o resultado de A + 0

(6CC0)h - 0110 110 011 000 0 00 - ARIT ADD, D, A, zero - armazena no registrador D o resultado de A + 0

(6DC0)h - 0110 110 111 000 0 00 - ARIT ADD, PSW, A, zero - armazena do registrador PSW o resultado de A + 0 (**não funciona!**)

(6D80)h - 0110 110 110 000 0 00 - ARIT ADD, R, A, zero - armazena no registrador R o resultado de A + 0

// carga de um registrador para o acumulador - verificado em 2024-04-11, funcionam

(6C08)h -  0110 110 000 001 0 00 - ARIT ADD, A, B, zero - armazena no registrador A o resultado de B + 0

(6C10)h - 0110 110 000 010 0 00 - ARIT ADD, A, C, zero - armazena no registrador A o resultado de C + 0

(6C18)h - 0110 110 000 011 0 00 - ARIT ADD, A, D, zero - armazena no registrador A o resultado de D + 0

(6C38)h -  0110 110 000 111 0 00 - ARIT ADD, A, PSW, zero - armazena no registrador A o resultado de PSW + 0

(6C30)h - 0110 110 000 110 0 00 - ARIT ADD, A, R, zero - armazena no registrador A o resultado de R + 0

// aritmetica e lógica

(6280)h - 0110 001 010 000 0 000 - ARIT F, C, X, X - armazena 0xFFFF no registrador C (*nota*: os operandos não são usados, então podem ser quaisquer. Neste caso usei os de bits zero para facilitar a codificação). ié C=0xFFFF

(6c05)h - 0110 110 000 000 1 01 - ARIT ADD, A, A, B - incrementa o conteúdo do registrador A pelo conteúdo do registrador B, ié, A+=B

(6605)h - 0110 011 000 000 1 01 - ARIT AND, A, A, B - calcula A = A & B - esta operação é importante para selecionar os bits do PSW a considerar num salto.

(6805)h - 0110 100 000 000 1 01 - ARIT OR, A, A, B - calcula A = A | B

(6000)h - 0110 000 000 000 0 000 ARIT ZERO, A, X, X - armazena 0x0 no registrador A (*nota*: os operandos não são usados, então podem ser quaisquer. Neste caso usei os de bits zero para facilitar a codificação). ié A=0x0

# Program Status Word (PSW)

PSW é um registrador de 16 bits que, no momento, tem 5 bits usados. Os bits são ajustados de acordo com o resultado da operação aritmética realizada. O PSW mantém o resultado da operação aritmética executada mais recentemente, de maneira que caso após uma operação aritmética ocorra qualquer quantidade de LDA, STA, JMP, JNP, o conteúdo do PSW continua inalterado. Por outro lado, se após uma operação aritmética o PSW for copiado para, por exemplo, o acumulador, a cópia é feita e (depois) o PSW é atualizado.

![](./Captura%20de%20tela%20de%202024-04-17%2020-32-27.png)


*over* = overflow na soma 

*under* = underflow na subtração 

*Op1>Op2*

*Op1==Op2*

*Op1<Op2*

### Exemplos de programas

Exemplo 1: testaMovRegAccEAccReg.mem

Exemplo 2: movRegAcc.mem

Exemplo 3:

x está no endereço 0x100 e vale 0xbeba = 0b1011111010111010
y está no endereço 0x102 e vale 0xc01a = 0b1100000000011010
z está no endereço 0x050 e recebe o resultado

z=y+x // deveria ser 0b0111 1110 1101 0100 (overflow) = 0x7ed4

psw=0x8888 = bits do PSW que podem interessar: overflow na soma, A<B

salta (para tratamento de erro) se overflow

como os sinais de controle de LDA e STA são diferentes de ARIT (em especial no decodificador de instruções), LDA e STA não alteram o conteúdo do PSW, logo, é possível intercalar operações aritméticas com STA e não perder o conteúdo de PSW. Isto é interessante para conservar o resultado de uma conta e, em seguida, checar o que ocorreu (ex. overflow).

programa em initEPSW.mem

circuito: CarregaArmazenaSaltaArit3.circ

Exemplo 4:

x está no endereço 0x100 e vale 0xbeba = 0b1011111010111010
y está no endereço 0x102 e vale 0xc01a = 0b1100000000011010
z está no endereço 0x050 e recebe o resultado


a constante 0xcaca está na posição 0x151
a constane 0x0001 está na posição 0x152
a constante 0x2000 está na posição 0x153

programa testado: overflowEesr-testado.mem , listagem abaixo

```
v2.0 raw
0 1100 6c40 1102 6c05 2050 6c78 1130
6605 4200 0 6040 6280 0 0 0
1100 6c40 1102 6c05 2050 6c38 3000 57*0
7ed4 175*0 beba c0ca c01a 501a 4*0 babe
0 c01a 50fa 4*0 beba c0ca 0 5e1a
4*0 babe c01a cac0 5*0 cac0 7*0 c01a
7*0 8000 c10a ca00 30*0 caca 1 2000
12*0 172 15*0 caca caca 142*0 1151 6c40
6c80 6cc0 1160 6c40 1153 6805 2220 1151
3220 1160 6c40 1152 6c05 2160 3204 15*0
2171 320b
```

Mnemônicos

```
	NOP
	LDA(100)                     // z=y+x
	ARIT ADD, B, A, zero         // z=y+x
	LDA(102)                     // z=y+x
	ARIT ADD, A, A, B            // z=y+x
	STA(050)                     // z=y+x
	ARIT ADD, B, PSW, zero       // if (overflow) ESR
	LDA(130)                     // if (overflow) ESR
	ARIT AND, A, A, B            // if (overflow) ESR
	JNZ 200  // salta para ESR   // if (overflow) ESR
	NOP
	ARIT ZERO, B, X, X           // comandos que não serão executados
	ARIT F, C, X, X
	NOP
	NOP
	NOP
	LDA(100)
	ARIT ADD, B, A, zero
	LDA(102)
	ARIT ADD, A, A, B
	STA(050)
	ARIT ADD, A, PSW, zero
	JMP 0

 beba                        // int y=0xbeba
 c0ca                        
 c01a                        // int x=0xc01a
  501a 4*0 babe
0 c01a 50fa 4*0 beba c0ca 0 5e1a
4*0 babe c01a cac0 5*0 cac0 7*0 c01a
7*0 
8000                         // #define OVERFLOW_MASK 0x8000 
c10a ca00 30*0 caca 
1                            // #define UM 0x0001
2000                         // #define STA_OPCODE 0x2000
12*0 172
200: LDA(151)          // carrega acc com 0xcaca
	ARIT ADD, B, A, zero
	ARIT ADD, C, A, zero
	ARIT ADD, D, A, zero
	204: LDA(160)          // prepara instrução 2000 | *p  do... while (1)
	ARIT ADD, B, A, zero   // prepara instrução 2000 | *p
	LDA(153)               // prepara instrução 2000 | *p
	ARIT OR, A, A, B       // prepara instrução 2000 | *p
	STA(220)               // prepara instrução 2000 | *p
	LDA(151)               // carrega acc com 0xcaca
	JMP 220                // salta para 220
20B: LDA(160)              //p=p+1 - p é um ponteiro para um inteiro!!
	ARIT ADD, B, A, zero   //p=p+1
	LDA(152)               //p=p+1
	ARIT ADD, A, A, B      //p=p+1
	STA(160)               //p=p+1
	JMP 204

220: 0000                  // nesta posição vai a instrução STA com endereço calculado. É código auto-modificante
	JMP 20B
```

O programa inicializa as constantes e aloca três variáveis (x, y, z). Inicializa x com 0xbeba e y com 0xco1a. Calcula a soma e armazena em z. Essa soma causa *overflow*. O PSW é lido e o bit de *overflow* é selecionado com o uso de uma máscara de bits. Caso o resultado seja diferente de zero, salta para a rotina de tratamento de erro (esr). A rotina de tratamento de erro preenche todos os registradores de uso geral com 0xcaca e executa um loop que a partir do endereço armazenado no endereço 0x160, preenche as palavras com 0xcaca. Em algum momento a rotina de tratamento de erro será sobrescrita.

## Tarefa (Exercício-Programa 1)

Escrever um programa em C que simula o simulador de processador a partir de `driverEP1.c` e `EP1.c`.

`driverEP1.c` contém funções para leitura e escrita dos *dumps* de memória RAM do logisim. Os protótipos das funções estão em `driverEP1.h`. Este arquivo de header contém também `processa(...)` o protótipo da função que simula o processador. Um esboço da implementação da função `processa(...)` é apresentado em `EP1.c`. O comando HALT está implementado.

Para compilar o exemplo faça `gcc driverEP1.c EP1.c`.

Formato de entrega: Envie apenas EP1.c através do e-disciplinas.

Método de teste: Diferentes `driverEP1.c` serão usados com diferentes conteúdos de memória. Depois da execução do programa o conteúdo da memória será comparado com o modelo de resposta (que resulta da execução do mesmo programa no simulador do logisim).

## Tarefa (Exercício-Programa 2)

Escolher uma arquitetura (ou usar alguma de algum computador que tenha acesso) e que tenha um compilador que gere assembly (como o gcc -S), tomar `ep2-1.c`  `ep2-2.c`  `ep2-3.c`, fragmentos simples de código, localizar as correspondências entre o código em C (declaração de variáveis, atribuições, comandos, chamadas de função) e os comandos dos respectivos códigos em assembly. Escreva um relatório apresentando as correspondências.

## Tarefa (Exercício-Programa 3)

Escrever um algoritmo de ordenação para ser executado no simulador de processador. Sugestão, usar um algoritmo simples como *insertion sort*, com implementação sem chamadas recursivas. Um recurso que pode ser usado é escrever o programa em C, compilar para a arquitetura que você estudou no EP2 e fazer as correspondências do assembly da arquitetura que você usou no EP2 para o assembly do simulador de processador (https://github.com/FNakano/OAC1/blob/main/meuProcessador/CarregaArmazenaSaltaArit4.circ)

