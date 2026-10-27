#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int abrir_ar(const char *ruta);
void cerrar_ar(void);
void *imprimir_des(void *arg);
void imprimir_asc(void);
int crear_hilo(pthread_t *hilo);
void esperar_hilo(pthread_t hilo);

static FILE *archivo=NULL;
static pthread_mutex_t candado=PTHREAD_MUTEX_INITIALIZER;

int main(){
  pthread_t hilo;
    if(abrir_ar("T2_SO_Hilos.txt")!=0)
    {
      return EXIT_FAILURE;
    }

    if(crear_hilo(&hilo)!=0)
    {
      cerrar_ar();
      return EXIT_FAILURE;
    }

  imprimir_asc();
  esperar_hilo(hilo);
  cerrar_ar();

  return EXIT_SUCCESS;
}

int abrir_ar(const char *ruta){
  archivo=fopen(ruta, "w");
    if(archivo==NULL)
    {
      perror("No se pudo abrir el archivo"); 
    }
 return 0;
}

void cerrar_ar(void){
     if(archivo!=NULL)
     {
       fclose(archivo);
       archivo=NULL;
     }
  pthread_mutex_destroy(&candado);
}

void *imprimir_des(void *arg){
   (void)arg;

     for(int i=10000; i>=1; i--)
     {
        pthread_mutex_lock(&candado);
        fprintf(archivo, "HILO %d \n", i);
        pthread_mutex_unlock(&candado);
     }
  pthread_exit(NULL);
}

void imprimir_asc(void){
     for(int i=1; i<=10000; i++)
     {
       pthread_mutex_lock(&candado);
       fprintf(archivo, "PRINCIPAL %d\n", i);
       pthread_mutex_unlock(&candado);
     }
}

int crear_hilo(pthread_t *hilo){
  int estado =pthread_create(hilo, NULL, imprimir_des, NULL);
    if(estado!=0)
    {
      fprintf(stderr, "Error al crear hilo: %d\n", estado);
      return -1;
    }
  return 0;
}


void esperar_hilo(pthread_t hilo){
  pthread_join(hilo, NULL);
}
