#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

#define SEGUNDO 1000000

typedef struct {
  char patente[5];
  int entrada;
  int peso;
  int tiempo;
}Vehiculo;

Vehiculo *leerArchivo(Vehiculo *autos,int *cant);

int main(int argc, char const *argv[]) {
  
  int aux;
  int maxweight = atoi(argv[1]);
  int kilo, n_auto, canautos, no_es_hijo, peso_puente, wpid;
  sem_t puente;
  sem_t en_puente; sem_t *ptr_en_pte = &en_puente;
  sem_init(&puente, 0, maxweight);
  sem_init(ptr_en_pte, 0, 0);
  
  Vehiculo *autos;
  autos = leerArchivo(autos,&canautos);

  //for(n_auto = 0; n_auto < canautos; n_auto++){
		
  	/*sem_getvalue(&puente, &peso_puente); 
  	printf("%d %s esperando | peso %d\n",n_auto, autos[n_auto].patente,peso_puente);*/    

		no_es_hijo = fork();
  
    if(no_es_hijo){
    	
			sem_getvalue(ptr_en_pte, &aux);
			printf("Antes wait %d\n",aux);
      sem_wait(ptr_en_pte);
			printf("test\n");
			sem_getvalue(ptr_en_pte, &aux);
			printf("Dsp wait %d\n",aux);

			/*sem_getvalue(&puente, &peso_puente);
			printf("%d %s en puente | peso %d\n",n_auto, autos[n_auto].patente,peso_puente);*/

      //if(n_auto != canautos-1) usleep( autos[n_auto+1].entrada * SEGUNDO);
    
    }else{
    	
      //for(kilo = 0; kilo < autos[n_auto].peso; kilo++) sem_wait(&puente);

			usleep( 5 * SEGUNDO );

			sem_getvalue(ptr_en_pte, &aux);
			printf("Antes post %d\n",aux);
			sem_post(ptr_en_pte);
			sem_getvalue(ptr_en_pte, &aux);
			printf("Dsp post %d\n",aux);

      //usleep( autos[n_auto].tiempo * SEGUNDO );

      //for(kilo = 0; kilo < autos[n_auto].peso; kilo++) sem_post(&puente);

			/*sem_getvalue(&puente, &peso_puente);
			printf("%d %s saliendo | peso %d\n",n_auto, autos[n_auto].patente,peso_puente);*/

			return 0;
    }

  //}

	while( (wpid = wait(NULL)) > 0 );
	
	sem_destroy(&puente);
  sem_destroy(ptr_en_pte);

  return 0;
}

Vehiculo *leerArchivo(Vehiculo *autos,int *cant){

  FILE *fp;
	fp = fopen( "vehiculos.txt","r");
	if (fp==NULL) {fputs ("File error",stderr); exit (1);}

	int ch,vehiculos,buffer1,i;
  char *buffer2= (char*)malloc(sizeof(char)*10);

  while ((ch = fgetc(fp)) != EOF)
      if (ch == '\n')
         vehiculos++;
  *cant=vehiculos;
  rewind(fp);

  autos=(Vehiculo*)malloc(sizeof(Vehiculo)*vehiculos);

  for( i = 0; i < vehiculos; i++ ){
        fscanf(fp,"%s",buffer2);
        strcpy(autos[i].patente,buffer2);
        fscanf(fp,"%d",&autos[i].entrada);
        fscanf(fp,"%d",&autos[i].peso);
        fscanf(fp,"%d",&autos[i].tiempo);
        }

    fclose(fp);
    return autos;
  }

/*---------------------------------------------------------------*/


	/* Codigo de prueba para imprimir autos
	int i;
	for( i = 0; i < canautos; i++ ){
        
        printf("%s %d %d %d\n",autos[i].patente,autos[i].entrada,autos[i].peso,autos[i].tiempo);

	}
	*/
