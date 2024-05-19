/*exploração sobre o algoritmo rápido para cálculo da raiz quadrada 
 * https://en.wikipedia.org/wiki/Fast_inverse_square_root#cite_note-FOOTNOTEMcEniry20073-29 
 * compilação: gcc verFloatUmTeste.c -lm 
 * uso: ./a.out 
 * resultado esperaado para o teste: 
 * a aprox. do log2 é 33.218864, 2 elevado à aproximação é 9997113403.550716. Erro: -0.235357
 **/
 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

union FLOATINT{float f; unsigned int i;}; // https://stackoverflow.com/questions/56435901/union-int-bits-to-float-bits-sometimes-interpreted-wrong

float aproximaLog (float f) {
/* espera-se que o argumento seja positivo. */

  union FLOATINT a;
  a.f=f;
  int exp, sig;

  exp= (a.i&0x7F800000)>>23; // oito bits depois do sinal, deslocado da quantidade de bits do significando;
  sig= a.i&0x007FFFFF; // 23 bits depois do expoente;

  exp-=127; // ajusta a notação enviesada;

  float dsig=(float)sig/pow(2,24);

  return dsig+exp;
}

int main () {
  float aproxLog=aproximaLog(1.235E10);
  printf ("a aprox. do log2 é %f, 2 elevado à aproximação é %f. Erro: %f\n", aproxLog, pow(2,aproxLog), 1.0-1.235e10/pow(2,aproxLog));
}
