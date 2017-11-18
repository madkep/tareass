#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct {
  char patente[5];
  int segundos;
  int peso;
  int tiempo;
}Autos;

int leerArchivo(Autos *autos);

int main(int argc, char const *argv[]) {
  int canautos;
  int maxweight = atoi(argv[1]);
  Autos *autos;
  canautos=leerArchivo(autos);

  printf("%d\n",canautos );
  printf("%d\n",autos[1].segundos);



  return 0;
}

int leerArchivo(Autos *aut){

  int ch,vehiculos,buffer1;
  char *buffer2= (char*)malloc(sizeof(char)*10);

  FILE *fp;
	fp = fopen( "vehiculos.txt","r");
	if (fp==NULL) {fputs ("File error",stderr); exit (1);}

  while ((ch = fgetc(fp)) != EOF)
      if (ch == '\n')
         vehiculos++;

  rewind(fp);
  aut = (Autos*) malloc(sizeof(Autos)*vehiculos);
  for( int i=0; i<5 ; i++ ){
        fscanf(fp,"%s",buffer2);
        strcpy(aut[i].patente,buffer2);
        fscanf(fp,"%d",&buffer1);
        aut[i].segundos=buffer1;
        fscanf(fp,"%d",&buffer1);
        aut[i].peso=buffer1;
        fscanf(fp,"%d",&buffer1);
        aut[i].tiempo=buffer1;
        printf("%s\n",aut[i].patente);
        }

    fclose(fp);
    return vehiculos;
  }
