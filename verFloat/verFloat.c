/*exploração sobre o algoritmo rápido para cálculo da raiz quadrada 
 * https://en.wikipedia.org/wiki/Fast_inverse_square_root#cite_note-FOOTNOTEMcEniry20073-29 
 * compila com: gcc verFloat.c -lm
 * Resultado esperado para execução:
 * 50380783
ssig=0, exp=50000000, sig=380783
ssig=0, exp=a0, sig=380783 (todos deslocados para unidade)
ssig=0, exp=21, sig=380783 (todos deslocados para unidade)
ssig=0, exp=33, dsig=0.218865 (todos deslocados para unidade)
a aprox. do log2 é 33.218864, 2 elevado à aproximação é 9997113403.550716. Erro: -0.235357
*/
 
#include <stdio.h>
#include <math.h>

union FLOATINT{float f; unsigned int i;}; // https://stackoverflow.com/questions/56435901/union-int-bits-to-float-bits-sometimes-interpreted-wrong

int main () {
  union FLOATINT a;
  //a.f=0.1235;
  a.f=1.235E10;
  printf("%x\n", a.i);
  int ssig, exp, sig;

  // sem deslocar...
  ssig=a.i&0x80000000; // bit mais significativo;
  exp= a.i&0x7F800000; // oito bits depois do sinal;
  sig= a.i&0x007FFFFF; // 23 bits depois do expoente;
  printf ("ssig=%x, exp=%x, sig=%x\n", ssig, exp, sig);
  
  // desloca para dar a escala correta
  ssig>>=31; // bit mais significativo;
  exp>>=23; // oito bits depois do sinal;
  // sig>>= 0; // 23 bits depois do expoente;
  printf ("ssig=%x, exp=%x, sig=%x (todos deslocados para unidade)\n", ssig, exp, sig);
  // o expoente é enviesado de 127, então "desenviesar"
  exp-=127;
  printf ("ssig=%x, exp=%x, sig=%x (todos deslocados para unidade)\n", ssig, exp, sig);
  // passando para uma base mais intuitiva - note que o significando é um binário fracionário então convertê-lo requer mudar o tipo para float e fazer algumas contas...
  float dsig=(float)sig/pow(2,24);
  printf ("ssig=%d, exp=%d, dsig=%f (todos deslocados para unidade)\n", ssig, exp, dsig);
  // Pergunta: exp+dsig é uma boa aproximação para log2(a.f)??
  double aproxLog=(float) exp+dsig;
  printf ("a aprox. do log2 é %f, 2 elevado à aproximação é %f. Erro: %f\n", aproxLog, pow(2,aproxLog), 1.0-a.f/pow(2,aproxLog));
}
