/*exploração sobre o algoritmo rápido para cálculo da raiz quadrada 
 * https://en.wikipedia.org/wiki/Fast_inverse_square_root#cite_note-FOOTNOTEMcEniry20073-29 
 * compilação: gcc verFloatAlgunsTestes.c -lm 
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
  for (int i=0;i<10;i++) {
    int e=rand()%38;
    float f=drand48();
    f=f*pow(10, e);     // coloca um expoente
    float aproxLog=aproximaLog(f);
    printf ("o número a tirar o log2 é %e, a aprox. do log2 é %f, 2 elevado à aproximação é %e. Erro: %f\n", f, aproxLog, pow(2,aproxLog), 1.0-f/pow(2,aproxLog));
  }
}
