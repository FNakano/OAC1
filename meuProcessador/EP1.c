#include "driverEP1.h"

int processa (short int *M, int memSize) {
  unsigned short int ri, pc, a, b, c, d, r, psw;
  pc=0;
  do {
    ri=M[pc];
    printf ("Ensine-me a fazer algo com %hx\n", ri);
    pc++;
    if (pc>=memSize) pc=0;
  } while ((ri&0xF000)!=0xF000);
}
