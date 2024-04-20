#include <stdio.h>
#include <string.h>

#define MAXNCHAR 1024
#define MAXMEMSIZE 4192
#define HEADER "v2.0 raw"

int leMem (FILE *fpIn);
int escreveMem (FILE *fpOut);
int processa (short int *M, int memsize);
