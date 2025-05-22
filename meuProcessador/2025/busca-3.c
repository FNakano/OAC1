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
