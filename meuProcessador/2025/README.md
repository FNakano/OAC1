## Planejar, escrever, executar um programa neste processador

### O resultado (para ver algo funcionando, mesmo que não entenda...

- Baixe e instale o logisim (https://cburch.com/logisim/pt/download.html);
- Baixe o arquivo que contém a descrição do processador: [CarregaArmazenaSaltaArit5.circ](../CarregaArmazenaSaltaArit5.circ);
- Baixe o arquivo que contém a imagem da memória [busca-5.mem](./busca-5.mem);
- Inicie o logisim;
- Carregue a descrição do processador
  - Menu *Arquivo->Abrir*;
  - Na caixa de diálogo localize o arquivo que contém a descrição do processador e abra-o;
- Carregue o arquivo que contém a imagem da memória na memória do processador
  - clique com o botão da direita sobre a memória e seleciona o ítem *Carregar Imagem*
  - Na caixa de diálogo localize o arquivo que contém a imagem da memória e abra-o;
- Execute o simulador
  - Selecione o modo de simulação (botão com mão com dedo para cima);
  - Clique no botão reset;
  - Selecione o ítem do menu *Simular->Pulso Habilitado* ou use o atalho CTRL-K.
  
Para executar a mesma simulação novamente, modifique o conteúdo do endereço de memória 170 (de volta para) 1100, clique no botão reset e inicie a simulação.

Segue explicação sobre o programa:

### Planejamento

#### Descrição e solução do problema em linguagem C

Em linguagem natural: Criar um programa que busca um valor em um array, caso o encontre, termina a execução.

Detalhes: O array tem 0x20 palavras (lembrando, 0xa palavra neste processador tem 16 bits), ele é armazenado a partir da posição de memória 0x100 (acredito que o programa não terá mais de 256 instruções).

Código em C (a fim de dar um direcionamento):
  
```c
/* busca-1.c */
#include <stdio.h>

int v[0x20] = {
0x3DB4, 0x337, 0x8B8, 0x2A31, 0x29ED, 0x29C6, 0x107F, 0x1919,
0x38B9, 0x3A1D, 0x3570, 0x379B, 0x396C, 0x3037, 0xFBD, 0xA15,
0x13D7, 0xDF0, 0x2EC2, 0x4F, 0x2114, 0x1507, 0x338F, 0x263D,
0x3C04, 0x1AA7, 0x1146, 0x1731, 0x68, 0x2F9, 0x1D80, 0x1F1F
};

int i=0, n=0x20, p=0x2114;

int main () {
   while ((i<n) && (v[i]!=p)) {
      i++;
   }
   // detalhes adicionais: se o valor procurado (p) não estiver no array v, ao final da execução i conterá n+1 onde n é o comprimento do array.
   printf ("%x\n", i);  // no processador, inspecionar o registrador ou posição de memória.
}

```

Agora um adendo bem longo sobre a arquitetura do processador:
  
### Arquitetura do processador

Este é um (simulador de) processador de 16 bits e sua memória principal de 4k palavras (8kBytes). O processador é o descrito no arquivo [CarregaArmazenaSaltaArit5.circ](../CarregaArmazenaSaltaArit5.circ). Abra-o usando o logisim (https://cburch.com/logisim/pt/download.html)

A notação dos números é (*big endian*) ié,  MSB primeiro.

A comunicação entre processador e memória é feita por um barramento de endereços de 12 bits, barramento de dados de 16 bits e sinais de controle.

O processador tem suporte para chamada de funções e retorno. 

Interrupções ainda não foram implementadas.

O processador tem 8 registradores ao todo dos quais 4 são de uso geral, nomeados A, B, C, D. Estes foram colocados no circuito para facilitar operações lógicas e aritméticas. Os outros 4 são:
  
  - armazenar em que endereço está a instrução que será/é executada, este é o contador de programa, ou program counter ou PC;
  - armazenar a instrução que está sendo executada, este é o regitrador de instrução ou RI; 
  - armazenar o endereço de retorno este é o RP;
  - armazenar o estado do programa, por exemplo se houve overflow em uma soma, ou underflow em uma subtração ou se um operando de uma operação aritmética é maior/menor/igual ao outro operando. Este regitrador é o program status word ou PSW;

#### Conjunto de Instruções


| OpCode  | Mnemônico | Descrição |
| ------  | --------- | --------- |
| (0000)b | NOP       | Não operação |
| (0001)b | LDA(X)    | Carrega conteúdo do acumulador com conteúdo armazenado no endereço X da memória |
| (0010)b | STA(X)    | Armazena no endereço X da memória o conteúdo do acumulador |
| (0011)b | JMP X     | Armazena o próximo endereço sequencial no registrador RP e **salta** execução para instrução armazenada no endereço X da memória |
| (0100)b | JNZ X     | Se o acumulador for diferente de zero, armazena o próximo endereço sequencial no registrador RP e salta execução para instrução armazenada no endereço X da memória |
| (0101)b | RET       | Armazena o próximo endereço sequencial no registrador RP e, simultaneamente, **salta** execução para endereço (anteriormente) armazenado no registrador RP |
| (0110)b | ARIT     | Executa operação aritmética a atualiza registrador PSW |
| (1111)b | HALT     | Para o processador ("desabilita o clock") |

Formato das instruções LDA, STA, JMP, JNZ, HALT:

![](../Captura%20de%20tela%20de%202024-04-17%2018-58-32.png)

Em uma palavra de memória, o código de operação (Op) ocupa os quatro bits mais significativos e o endereço de memória (X) ocupa os 12 bits restantes.

Na operação LDA o conteúdo da memória no endereço X é carregado no acumulador (registrador A).

Na operação STA o conteúdo do acumulador (registrador A) é armazenado no endereço X da memória.

Nas instruções JMP e JNZ, X é o endereço da memória onde está armazenada a próxima instrução caso o salto seja executado. Caso o salto não seja executado a próxima instrução é a do endereço seguinte do endereço onde está armazenada a instrução de salto. Ainda caso o salto seja executado, o endereço seguinte do endereço da instrução de salto é armazenado no registrador R.

Exemplos:

(1120)h = LDA(120) - carrega o acumulador com o valor contido no endereço 0x120;

(2151)h = STA(151) - armazena o valor contido no acumulador no conteúdo do endereço 0x120;

(3010)h = JMP 010 - salta para o endereço 0x010, ié passa a executar a partir da instrução armazenada no endereço 0x010;

(4010)h = JNZ 010 - Se o registrador A contiver valor diferente de zero, salta para o endereço 0x010, ié passa a executar a partir da instrução armazenada no endereço 0x010, caso contrário segue executando a instrução no endereço seguinte ao da instrução de salto;
 

![](../Captura%20de%20tela%20de%202024-04-17%2019-32-08.png)

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

(6000)h - 0110 000 000 000 0 000 - ARIT ZERO, A, X, X - armazena 0x0 no registrador A (*nota*: os operandos não são usados, então podem ser quaisquer. Neste caso usei os de bits zero para facilitar a codificação). ié A=0x0

(6C78)h - 0110 110 001 111 0 00 - ARIT ADD, B, PSW, zero - transfere o conteúdo de PSW para B

(660C)h - 0110 011 000 001 1 00 - ARIT AND, A, B, A - E bit a bit entre A e B e armazena o resultado em A 

# Program Status Word (PSW)

PSW é um registrador de 16 bits que, no momento, tem 5 bits usados. Os bits são ajustados de acordo com o resultado da operação aritmética realizada. O PSW mantém o resultado da operação aritmética executada mais recentemente, de maneira que caso após uma operação aritmética ocorra qualquer quantidade de LDA, STA, JMP, JNP, o conteúdo do PSW continua inalterado. Por outro lado, se após uma operação aritmética o PSW for copiado para, por exemplo, o acumulador, a cópia é feita e (depois) o PSW é atualizado.

![](../Captura%20de%20tela%20de%202024-04-17%2020-32-27.png)


*over* = overflow na soma 

*under* = underflow na subtração 

*Op1>Op2*

*Op1==Op2*

*Op1<Op2*

... agora segue o desenvolvimento do código-fonte do programa considerando a arquitetura e organização do processador, passos são dados em direção a aproximar-se do programa em assembly/linguagem de máquina tendo em vista a arquitetura...

### Passo 2

O processador tem quatro registradores de uso geral, o programa tem 3 variáveis inteiras. Provavelmente precisará armazenar variáveis na memória pois deve haver resultados intermediários ou operações feitas por modificar o PSW (resultado descartado), ...

Na arquitetura-alvo, o acumulador é o único registrador que carrega dados diretamente da memória, convém que ele armazene v[i] na execução da comparação. A comparação com p é muito frequente, convém que p esteja em outro registrador. i é incrementado frequentemente mas precisa ser escrito na memória para ser possível ler o array. Considerando estes detalhes, convém definir que o último elemento do array será 0x0000 (ao invés de fixar-lhe um comprimento) e definir constantes. O programa em C fica:
  
```c
/* busca-2.c */
#include <stdio.h>
#define UM 1
#define P 0x2114
#define SENTINELA 0x0000

int v[0x20] = {
0x3DB4, 0x337, 0x8B8, 0x2A31, 0x29ED, 0x29C6, 0x107F, 0x1919,
0x38B9, 0x3A1D, 0x3570, 0x379B, 0x396C, 0x3037, 0xFBD, 0xA15,
0x13D7, 0xDF0, 0x2EC2, 0x4F, 0x2114, 0x1507, 0x338F, 0x263D,
0x3C04, 0x1AA7, 0x1146, 0x1731, 0x68, 0x2F9, 0x1D80, 0x0000
};

int i=0;

int main () {
   while ((v[i]!=SENTINELA) && (v[i]!=P)) {
      i++;
   }
   // detalhes adicionais: ao final da execução i conterá ou a posição de p ou a posição de SENTINELA.
   printf ("%x\n", i);  // no processador, inspecionar o registrador ou posição de memória.
}

```

### Passo 3

Incluindo os registradores e considerando as instruções de desvio de fluxo da arquitetura, o que leva a usar do...while(1) ...

```c
/* busca-3.c */
#include <stdio.h>
// as constantes abaixo estão em algum lugar na memória
#define UM 1
#define P 0x2114
#define SENTINELA 0x0000 // este não precisa estar na memória pois está disponível na ULA

int v[0x20] = {
0x3DB4, 0x337, 0x8B8, 0x2A31, 0x29ED, 0x29C6, 0x107F, 0x1919,
0x38B9, 0x3A1D, 0x3570, 0x379B, 0x396C, 0x3037, 0xFBD, 0xA15,
0x13D7, 0xDF0, 0x2EC2, 0x4F, 0x2114, 0x1507, 0x338F, 0x263D,
0x3C04, 0x1AA7, 0x1146, 0x1731, 0x68, 0x2F9, 0x1D80, 0x0000
};

int i=0;          // esta variável está em algum lugar da memória  

int a, b, c, d;   // registradores do processador

int main () {
  // inicialização
  a=P;
  d=a;  // o registrador d conterá o valor buscado
  do {
    a=i;
    a=v[a];
    if (a==SENTINELA) break;
    if (a==d) break;
    i=i+UM;
   } while (1);
   // detalhes adicionais: ao final da execução i conterá ou a posição de p ou a posição de SENTINELA.
   printf ("%x\n", i);  // no processador, inspecionar o registrador ou posição de memória.
}

```

### Passo 4


SENTINELA poderia ser armazenada em um registrador... e detalhando as cargas de acumulador (pois só com o acumulador que se escreve valores na memória) ...

```c
/* busca-4.c - agora resolve coisas relativas à arquitetura...*/
#include <stdio.h>
// as constantes abaixo estão em algum lugar na memória
#define UM 1
#define P 0x2114
#define SENTINELA 0x0000 // este não precisa estar na memória pois está disponível na ULA

int v[0x20] = { //3DB4
0x3DB4, 0x337, 0x8B8, 0x2A31, 0x29ED, 0x29C6, 0x107F, 0x1919,
0x38B9, 0x3A1D, 0x3570, 0x379B, 0x396C, 0x3037, 0xFBD, 0xA15,
0x13D7, 0xDF0, 0x2EC2, 0x4F, 0x2114, 0x1507, 0x338F, 0x263D,
0x3C04, 0x1AA7, 0x1146, 0x1731, 0x68, 0x2F9, 0x1D80, 0x0000
};

short int i=0;          // esta variável está em algum lugar da memória  

short int a, b, c, d;   // registradores do processador

int main () {
  // inicialização
  a=P;
  d=a;  // o registrador d conterá o valor buscado
  a=SENTINELA;
  c=a;
  do {
    a=v[i]; 
    if (a==c) break;  
    a=v[i];
    if (a==d) break;
    a=UM;
    b=a;
    a=i;
    a=a+b;
    i=a;
   } while (1);
   // detalhes adicionais: ao final da execução i conterá ou a posição de p ou a posição de SENTINELA.
   printf ("%x\n", i);  // no processador, inspecionar o registrador ou posição de memória.
}


```


No processador `A==C` é computado quando `A` e `C` são operandos de uma operação aritmética qualquer, o resultado da comparação é armazenado no PSW e deve ser lido para o acumulador na operação seguinte.

Resolvendo posição das constantes na memória: P fica no endereço 0x150, SENTINELA no endereço 0x151, UM no endereço 0x152. A máscara para o flag de `==` é 0x1000, armazenado no endereço 0x153.

Traduzindo para assembly (ié linguagem de montagem) 

```
LDA 150
ADD D A ZERO
LDA 151
ADD C A ZERO

JMP 170    // executa palavra armazenada no endereço 170
ADD B A C 
ADD B PSW ZERO
LDA 153    // MASCARA DO ==
AND A B A
JNZ 160

JMP 170    // executa palavra armazenada no endereço 170
ADD B A D 
ADD B PSW ZERO
LDA 153     // MASCARA DO ==
AND A B C
JNZ 160

LDA 152
ADD B A ZERO
LDA 170
ADD A A B
STA 170    // armazena o valor contido em A no endereço 170

JMP 004
```

No endereço 170 a rotina (função) que lê um elemento do array e armazena no acumulador

```
LDA 100    // a palavra correspondente a esta instrução é dado ou é instrução?
RET
```

No endereço 160 a rotina (função) que para o processamento pois encontrou p ou chegou ao fim do array

```
HALT
```

**Veja seção bônus!!**

## Passo 5

Converter cada linha do programa em assembly para o correspondente código da instrução em linguagem de máquina.

**NOTA**: usar o salvar imagem e o carregar imagem clicando sobre o bloco de memória. O salvar do menu Arquivo não funciona.

### Cálculos auxiliares

0b0110 1100 0100 0110 = 6C46 - b=a+c - ARIT ADD b a c (colado da tabela de exemplos)

(bits do PSW) cout bout < = > 0 0 0 0 0 0 0 0 0 0 0

ARIT ADD, B, PSW, zero 0110 110 001 111 0 00 = 6C78 (montado a partir da especificação da operação)

ARIT AND, A, B, A 0110 011 000 001 1 00 = 660C (montado a partir da especificação da operação)

Programa em linguagem de máquina (busca-5.mem)

```
v2.0 raw
1150 6cc0 1151 6c80 3170 6c46 6c78 1153
660c 4160 3170 6c47 6c78 1153 660c 4160
1152 6c40 1170 6c05 2170 3004 234*0 3db4
337 8b8 2a31 29ed 29c6 107f 1919 38b9
3a1d 3570 379b 396c 3037 fbd a15 13d7
df0 2ec2 4f 2114 1507 338f 263d 3c04
1aa7 1146 1731 68 2f9 1d80 49*0 2114
0 1 1000 12*0 f000 15*0 1100 5000

```

## Bônus

Compare o assembly gerado por gcc para x86 e para exynos2400e (ARM) na compilação de `busca-4.c`

[arquivo assembly para x86](./busca-4.s.x86) - compilado com gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

[arquivo assembly para exynos2400e (ARM)](./busca-4.s.exy)- compilado com clang version 19.1.7 target aarch64-unknown-linux-android29 thread model: posix, executado dentro do Termux versão googleplay.2025.01.18


