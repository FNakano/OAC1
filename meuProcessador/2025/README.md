## Planejar, 0xescrever, 0xexecutar um programa neste processador

### Planejamento

Em linguagem natural: Criar um programa que busca um valor em um array, 0xcaso o encontre, 0xtermina a execução.

Detalhes: O array tem 0x20 palavras (lembrando, 0xa palavra neste processador tem 16 bits), 0xele é armazenado a partir da posição de memória 0x100 (acredito que o programa não terá mais de 256 instruções).

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

O processador tem quatro registradores de uso geral, o programa tem 3 variáveis inteiras. Provavelmente precisará armazenar variáveis na memória.

Na arquitetura-alvo, o acumulador é o único registrador que carrega dados diretamente da memória, convém que ele armazene v[i] na execução da comparação. A comparação com p é muito frequente, convém que p esteja em outro registrador. i é incrementado frequentemente. Considerando estes detalhes, convém definir que o último elemento do array será 0x0000 e definir constantes. O programa em C fica:
  
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
   // detalhes adicionais: se o valor procurado (p) não estiver no array v, ao final da execução i conterá n+1 onde n é o comprimento do array.
   printf ("%x\n", i);  // no processador, inspecionar o registrador ou posição de memória.
}

```

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
   // detalhes adicionais: se o valor procurado (p) não estiver no array v, ao final da execução i conterá n+1 onde n é o comprimento do array.
   printf ("%x\n", i);  // no processador, inspecionar o registrador ou posição de memória.
}

```

SENTINELA poderia ser armazenada em um registrador... e detalhando as cargas de acumulador...

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
   // detalhes adicionais: se o valor procurado (p) não estiver no array v, ao final da execução i conterá n+1 onde n é o comprimento do array.
   printf ("%x\n", i);  // no processador, inspecionar o registrador ou posição de memória.
}


```


No processador `a==c` é computado quando `a` e `c` são operandos de uma operação aritmética qualquer, o resultado da comparação é armazenado no PSW e deve ser lido para o acumulador na operação seguinte.

Resolvendo posição das constantes na memória: P fica no endereço 0x150, SENTINELA no endereço 0x151, UM no endereço 0x152. A máscara para o flag de `==` é 0x1000, armazenado no endereço 0x153.

```
LDA 150
ADD d a ZERO
LDA 151
ADD c a ZERO

JMP 170
ADD b a c 
ADD b PSW ZERO
LDA 153    // MASCARA DO ==
AND a b a
JNZ 160

JMP 170
ADD b a d 
ADD b PSW ZERO
LDA 153     // MASCARA DO ==
AND a b a
JNZ 160

LDA 152
ADD b a ZERO
LDA 170
ADD a a b
STA 170

JMP 004
```

No endereço 170 a rotina (função) que lê um elemento do array e armazena no acumulador

```
LDA 100
RET
```

No endereço 160 a rotina (função) que para o processamento pois encontrou p ou chegou ao fim do array

**NOTA**: usar o salvar imagem e o carregar imagem clicando sobre o bloco de memória. O salvar do menu Arquivo não funciona.

Cálculos

0b0110 1100 0100 0110 = 6C46 - b=a+c - ARIT ADD b a c 

cout bout < = > 0 0 0 0 0 0 0 0 0 0 0

ARIT ADD, B, PSW, zero 0110 110 001 111 0 00 = 6C78

ARIT AND, A, B, A 0110 011 000 001 1 00 = 660C

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

O processador usado é CarregaArmazenaSaltaArit5.circ
