#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


sem_t sem_aux1;
sem_t sem_aux2;
sem_t sem_impr;
FILE *fp1;

struct semaforos {
	sem_t sem_cortarIngredientes;
	sem_t sem_cortarExtras;
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_armarMed;
	sem_t sem_hornearPan;
	sem_t sem_cocinarMed;
	sem_t sem_armarHam;
};

struct paso {
	char accion [50];
	char ingredientes [4][50];
};

struct parametro {
	int equipo_param;
	struct semaforos sem_param;
	struct paso pasos_param[8];
};

void* imprimirAccion(void *data, char *accionIn) {
	
	
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		fprintf(fp1, "\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
//		fclose(fp1);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
//		fprintf(fp1,"\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
							//fprintf(fp1,"\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
	
	
}

void* cortarIngredientes(void *data) {
	//creo el nombre de la accion de la funcion 
	
		struct parametro *mydata = data;
		char *accion = "cortarI";
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );
		sem_post(&mydata->sem_param.sem_mezclar);
		
		pthread_exit(NULL);
};



void* mezclar (void *data) {
		
		char *accion = "mezclar";
		struct parametro *mydata = data;
		
		sem_wait(&mydata->sem_param.sem_mezclar);
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );
		sem_post(&mydata->sem_param.sem_salar);
		
		pthread_exit(NULL);
};

void* salar (void *data) {

		char *accion = "salar";
		struct parametro *mydata = data;
		
		sem_wait(&mydata->sem_param.sem_salar);
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );
		sem_post(&mydata->sem_param.sem_salar);
		sem_post(&mydata->sem_param.sem_armarMed);

		pthread_exit(NULL);
};

void* armarMed (void *data) {

		char *accion = "armarM";
		struct parametro *mydata = data;
		
		sem_wait(&mydata->sem_param.sem_armarMed);
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );
		sem_post(&mydata->sem_param.sem_cocinarMed);
		
		pthread_exit(NULL);
};

void* hornearPan (void *data) {

		char *accion = "hornear";
		struct parametro *mydata = data;
		
		sem_wait(&sem_aux1);
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );
		sem_post(&mydata->sem_param.sem_hornearPan);
		sem_post(&sem_aux1);

		pthread_exit(NULL);
};


void* cocinarMed (void *data) {

		char *accion = "cocinar";
		struct parametro *mydata = data;
		
		sem_wait(&mydata->sem_param.sem_cocinarMed);
		sem_wait(&sem_aux2);
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );
		sem_post(&sem_aux2);
		sem_post(&mydata->sem_param.sem_cocinarMed);
		
		pthread_exit(NULL);
};

void* cortarExtras (void *data) {

		char *accion = "cortarE";
		struct parametro *mydata = data;
		
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );

		pthread_exit(NULL);	
};

void* armarHam(void *data) {
	
		char *accion = "armarH";
		struct parametro *mydata = data;

		sem_wait(&mydata->sem_param.sem_cocinarMed);
		sem_wait(&mydata->sem_param.sem_hornearPan);
		sem_wait(&mydata->sem_param.sem_cortarExtras);
		sem_wait(&sem_impr);
		imprimirAccion(mydata,accion);
		sem_post(&sem_impr);
		usleep( 20000 );
		
		pthread_exit(NULL);
};  

void* ejecutarReceta(void *i) {

	fp1 = fopen("resultado" , "w+");
	sem_t sem_cortarIngredientes;
	sem_t sem_cortarExtras;
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_armarMed;
	sem_t sem_hornearPan;
	sem_t sem_cocinarMed;
	sem_t sem_armarHam;

								// hilos de pasos;
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;

	int res;
	//Semaforos auxiliares
	res = sem_init(&sem_aux1,0,1);
	res = sem_init(&sem_aux2,0,1);
	res = sem_init(&sem_impr,0,1);
	
	int equipo = *((int*) i);
	
	printf("Ejecutando equipo %d \n", equipo);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));
	
	pthread_data->equipo_param = equipo;
	pthread_data->sem_param.sem_cortarIngredientes = sem_cortarIngredientes;
	pthread_data->sem_param.sem_cortarExtras = sem_cortarExtras;
	pthread_data->sem_param.sem_mezclar = sem_mezclar;
	pthread_data->sem_param.sem_salar = sem_salar;
	pthread_data->sem_param.sem_armarMed = sem_armarMed;
	pthread_data->sem_param.sem_hornearPan = sem_hornearPan;
	pthread_data->sem_param.sem_cocinarMed = sem_cocinarMed;
	pthread_data->sem_param.sem_armarHam = sem_armarHam;
	
	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
	char c[1000];
	char *receta = malloc (sizeof (char) * 1000);
	int ret;
	int b;
	int x;
	x = 0;
	FILE *fp;
	fp = fopen ( "receta.txt", "r" );
	if (fp==NULL){
		printf("%s", "error");
	}else {
		while (ret = fscanf(fp, "%s", c)) {
			
			if (ret == EOF) {
				break;
			}
			else {
				b = 0;
				char *token = strtok(c, "| \n");
				strcpy(pthread_data->pasos_param[x].accion, token);

				
				while ((token = strtok(NULL, "| \n")) != NULL) {
					strcpy(pthread_data->pasos_param[x].ingredientes[b], token);
					b +=1;
				}
				x +=1;		
			}
		}
	fclose(fp);
	}
	
	sem_init(&pthread_data->sem_param.sem_cortarIngredientes,0,1);
	sem_init(&pthread_data->sem_param.sem_cortarExtras,0,1);
	sem_init(&pthread_data->sem_param.sem_mezclar,0,0);
	sem_init(&pthread_data->sem_param.sem_salar,0,0);
	sem_init(&pthread_data->sem_param.sem_armarMed,0,0);
	sem_init(&pthread_data->sem_param.sem_hornearPan,0,1);
	sem_init(&pthread_data->sem_param.sem_cocinarMed,0,0);
	sem_init(&pthread_data->sem_param.sem_armarHam,0,0);
	
	pthread_create(&p1,NULL,cortarIngredientes,pthread_data);
	pthread_create(&p2,NULL,cortarExtras,pthread_data);
        pthread_create(&p3,NULL,mezclar,pthread_data); 
	pthread_create(&p4,NULL,salar,pthread_data); 
	pthread_create(&p5,NULL,armarMed,pthread_data); 
	pthread_create(&p6,NULL,hornearPan,pthread_data); 
	pthread_create(&p7,NULL,cocinarMed,pthread_data); 
	
	pthread_create(&p8,NULL,armarHam,pthread_data); 
	
	
	//crear demas hilos aqui
	
	
	//join de todos los hilos
	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);
	
	sem_destroy(&sem_cortarIngredientes);
	sem_destroy(&sem_cortarExtras);
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_armarMed);
	sem_destroy(&sem_hornearPan);
	sem_destroy(&sem_cocinarMed);
	sem_destroy(&sem_armarHam);
	//destruir demas semaforos 
	
	//salida del hilo
	pthread_exit(NULL);
	
	fclose(fp1);
}	

int main() {
	
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	
	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;
	
	pthread_create(&equipo1,NULL, *ejecutarReceta,equipoNombre1);
	pthread_create(&equipo2,NULL, *ejecutarReceta,equipoNombre2);
	pthread_create(&equipo3,NULL, *ejecutarReceta,equipoNombre3);
	
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	
	pthread_exit(NULL);
	
	return 0;
}
