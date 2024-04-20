/* driverEP1.c
 * Arquivo que contém funções de leitura/escrita de arquivos e a função main.
 * Deve ser compilado junto com EP1.c que deve conter sua implementação do simulador.
 * Função para leitura e para escrita do arquivo que armazena o conteúdo de uma memória RAM no logisim 
Exemplo:
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
*/

#include "driverEP1.h"

char bufferDeLinha[MAXNCHAR];
int nchar;

unsigned short int M[MAXMEMSIZE];  // unsigned 16-bit
int memSize;

int leMem (FILE *fpIn){
  memSize=0;
  int lineNumber=1;
  fgets (bufferDeLinha, MAXNCHAR, fpIn);
  if (strncmp (bufferDeLinha, HEADER, 8)) {
	// strcmp returns zero (false) if strings are equal
	puts ("Logisim RAM file header not found, exiting read.");
	printf ("line read: %s\n", bufferDeLinha);
	return -1;
  }
  while (!feof(fpIn)) {
    fgets (bufferDeLinha, MAXNCHAR, fpIn);
    lineNumber++;
    char *token;
    int rep, val;
    token=strtok (bufferDeLinha, " \n\t");
    while (token) {
		// puts (token);
		if (sscanf (token, "%d*%x", &rep, &val)==2) {
		  for (int i=0;i<rep;i++) {
			M[memSize]=val;
			memSize++;
		  }
		} else {
		  if (sscanf(token, "%x", &val)==1) {
			M[memSize]=val;
			memSize++;
          } else {
		    printf ("syntax error reading line %d.\n", lineNumber);
		    printf ("last token read %s.\n", token);
		    printf ("last memory address read %x.\n", memSize);
		    puts ("exiting read.");
		    return -1;
		} 
	  }
      token=strtok (NULL, " \n\t");
	} // while token
  }
  fclose (fpIn);
  return 0;
}

int escreveMem (FILE *fpOut) {
  fputs (HEADER, fpOut);
  for (int i=0;i<memSize;i++) {
	if ((i%8)==0) fputc('\n', fpOut);
	else if ((i%4)==0) fputc('\t', fpOut);
    fprintf (fpOut, "%hx ", M[i]);
  }
  return 0;
}

int main (int argc, char *argv[]) {
  if ((argc==2)||(argc==3)) {
    FILE *fpIn=fopen (argv[1], "rt");
    leMem(fpIn);
    processa (M, memSize);
    if (argc==2) escreveMem(stdout);
    else {
      FILE *fpOut=fopen (argv[2], "wt");
      escreveMem(fpOut);
    }
  } else {
     puts ("Read and write files containing logisim RAM content.");
     puts ("Usage: ./a.out <input filename> [output filename]");
  }
  return 0;
}
