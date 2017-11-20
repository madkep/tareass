/*

Felipe pon tus datos aca q amime da paja
Vicente Fernández V.			19.619.730-3

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>

//un segundo en microsegundos
#define SEGUNDO 1000000

//se dejan los ids de los semaforos en memoria compartida
sem_t *puente;
sem_t *en_pte;

//se define un tipo de dato vehiculo
typedef struct {
  char patente[5];
  int entrada;
  int peso;
  int tiempo;
}Vehiculo;

//lee el archivo vehiculos.txt y coloca los vehiculos en el arreglo autos
Vehiculo *leerArchivo(Vehiculo *autos,int *cant);

//se encargan de incrementar y decrementar el peso actual del puente
void entrar_puente(int peso);
void salir_puente(int peso);

int main(int argc, char const *argv[]) {
  
	//declaracion de variables
  int maxweight = atoi(argv[1]);
	int n_auto, canautos, no_es_hijo, peso_puente, wpid;

	//creacion de los semaforos
  puente = sem_open("puente", O_CREAT, 0644, maxweight);
  en_pte = sem_open("en_pte", O_CREAT, 0644, 0);
  
	//se crea un arreglo y se carga de vehiculos
  Vehiculo *autos;
  autos = leerArchivo(autos,&canautos);

	//un for para repetir el siguiente proceso por cada vehiculo
  for(n_auto = 0; n_auto < canautos; n_auto++){

		//espera la cantidad de segundos que entra despues del vehiculo anterior
		usleep(autos[n_auto].entrada * SEGUNDO);
		
		//se obtiene el peso actual del puente y se imprime por pantalla
		sem_getvalue(puente, &peso_puente); 
		printf("[Vehiculo]: %s entrando  [carga actual del puente]: %d toneladas\n", autos[n_auto].patente,maxweight-peso_puente);

		//para crear el proceso hijo que representara al vehiculo pasando por el puente
		no_es_hijo = fork();
  
		//proceso padre
    if(no_es_hijo){
    	
			//espera a que el vehiculo (hijo) entre al puente
			sem_wait(en_pte);

			//se obtiene el peso actual del puente y se imprime por pantalla
			sem_getvalue(puente, &peso_puente);
			printf("[Vehiculo]: %s en puente [carga actual del puente]: %d toneladas\n", autos[n_auto].patente,maxweight-peso_puente);
    
		//proceso hijo
    }else{

			entrar_puente(autos[n_auto].peso);

			//envia señal al padre para indicar que ya entro al puente
			sem_post(en_pte);

			//se espera el tiempo que el vehiculo esta en el puente
      usleep( autos[n_auto].tiempo * SEGUNDO );

      salir_puente(autos[n_auto].peso);

			//se obtiene el peso actual del puente y se imprime por pantalla
			sem_getvalue(puente, &peso_puente);
			printf("[Vehiculo]: %s saliendo  [carga actual del puente]: %d toneladas\n", autos[n_auto].patente,maxweight-peso_puente);

			//se termina el proceso hijo
			return 0;
    }

  }

	//se esperan a todos los procesos hijos
	while( (wpid = wait(NULL)) > 0 );
	
	//se eliminan los semaforos
	sem_unlink("puente");
	sem_close(puente);
  sem_unlink("en_pte");
  sem_close(en_pte);

  return 0;
}

void entrar_puente(int peso){

	int kilo;

	for(kilo = 0; kilo < peso; kilo++) sem_wait(puente);
	
}

void salir_puente(int peso){

	int kilo;

	for(kilo = 0; kilo < peso; kilo++) sem_post(puente);
	
}

Vehiculo *leerArchivo(Vehiculo *autos,int *cant){

	//se abre el archivo, en caso de fallar, se termina el programa
  FILE *fp;
	fp = fopen( "vehiculos.txt","r");
	if (fp==NULL) {fputs ("File error",stderr); exit (1);}

	//creacion de variables
	int ch,vehiculos,buffer1,i;
  char *buffer2 = (char*)malloc(sizeof(char)*10);

	//se cuenta la cantidad de vehiculos
  while ((ch = fgetc(fp)) != EOF)
      if (ch == '\n')
         vehiculos++;
  *cant = vehiculos;
  rewind(fp);
	
	//se reserva la memoria para el arreglo de vehiculos
  autos = (Vehiculo*)malloc(sizeof(Vehiculo)*vehiculos);

	//se ingresan los vehiculos al arreglo
  for( i = 0; i < vehiculos; i++ ){
        fscanf(fp,"%s",buffer2);
        strcpy(autos[i].patente,buffer2);
        fscanf(fp,"%d",&autos[i].entrada);
        fscanf(fp,"%d",&autos[i].peso);
        fscanf(fp,"%d",&autos[i].tiempo);
  }

	//se cierra el archivo y se retorna el arreglo de vehiculos
  fclose(fp);
  return autos;
 }
