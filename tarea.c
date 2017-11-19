#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

typedef struct {
  char patente[5];
  int segundos;
  int peso;
  int tiempo;
}Autos;

Autos *leerArchivo(Autos *aut,int *cant);

int main(int argc, char const *argv[]) {
  
  int maxweight = atoi(argv[1]);
  int kilo, car, canautos, no_es_hijo;
  Autos *autos;
  autos=leerArchivo(autos,&canautos);

  sem_t peso;
  sem_t en_puente;
  sem_init(&peso, 0, maxweight);
  sem_init(&en_puente, 0, 1);
	
  for(car = 0; car < canautos; car++){

    no_es_hijo = fork();
  
    if(no_es_hijo){
    
      sem_wait(&en_puente);
      if(car != canautos-1) usleep( autos[car+1].segundos * pollingDelay * 10000 );
    
    }else{
    
      for(kilo = 0; kilo < autos[car].peso; kilo++) sem_wait(&peso);
      sem_signal(&en_puente);
      usleep( autos[car].tiempo * pollingDelay * 10000 );
      for(kilo = 0; kilo < autos[car].peso; kilo++) sem_signal(&peso);
    
    }

  }

  return 0;
}

Autos *leerArchivo(Autos *aut,int *cant){

  int ch,vehiculos,buffer1;
  char *buffer2= (char*)malloc(sizeof(char)*10);

  FILE *fp;
	fp = fopen( "vehiculos.txt","r");
	if (fp==NULL) {fputs ("File error",stderr); exit (1);}

  while ((ch = fgetc(fp)) != EOF)
      if (ch == '\n')
         vehiculos++;
  *cant=vehiculos;
  rewind(fp);

  aut=(Autos*)malloc(sizeof(Autos)*vehiculos);

  for( int i=0; i<vehiculos ; i++ ){
        fscanf(fp,"%s",buffer2);
        strcpy(aut[i].patente,buffer2);
        fscanf(fp,"%d",&buffer1);
        aut[i].segundos=buffer1;
        fscanf(fp,"%d",&buffer1);
        aut[i].peso=buffer1;
        fscanf(fp,"%d",&buffer1);
        aut[i].tiempo=buffer1;
        }

    fclose(fp);
    return aut;
  }
