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
