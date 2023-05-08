//%cflags:-lpthread -lm

/** 	compilar con math.h   -lm
** 	compilar con threads: -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PROCESOS 1
/* Variables para semaforos*/
sem_t *semaforo;

struct datosCalc{
	long start;
	long iterations;
  double valor;
}


* calculoPi(void *datos)
{
	struct datosCalc *datos_proceso;
	datos_proceso = (struct datosCalc *) datos;
  
  double pi = 0.0;

  sem_wait(semaforo);

  for (long i = datos_proceso -> start; i < datos_proceso -> iterations; i++) {
    double term = (i % 2 == 0) ? 1.0 : -1.0;
    term /= (2 * i + 1);
    pi += 4.0 * term;
  }

  datos_proceso -> valor = pi;

  sem_post(semaforo);
}


int main()
{
  const long ITERS = 1e11;
  const int CANT_THREADS = 16;

	int error;
	char *valor_devuelto;
	/* Variables para hilos*/
  struct datosCalc threadsData[CANT_THREADS]; 
  pthread_t threadsId[CANT_THREADS];
	struct datosCalc hilo1_datos, hilo2_datos;
	pthread_t idhilo1, idhilo2, idhilo3, idhilo4;

	hilo1_datos.start = 0;
	hilo1_datos.iterations = ITERS/2;

	hilo2_datos.start = ITERS/2;
	hilo2_datos.iterations = ITERS;

	semaforo = sem_open("semaforo_name", O_CREAT, 0700, MAX_PROCESOS);

  for (int i = 0; i < CANT_THREADS; i++) {

    threadsData[i].start = i * ITERS/CANT_THREADS;

    threadsData[i].iterations = ITERS/(CANT_THREADS-i);

  }

  for (int i = 0; i < CANT_THREADS; i++) {
    error = pthread_create(&threadsId[i], NULL, (void *)calculoPi, (void *)(&threadsData[i]));
	  if (error != 0)
	  {
      perror ("No puedo crear hilo");
	 	  exit (-1);
	  }
    pthread_join(threadsId[i], (void **)&valor_devuelto);
  }


  /*
	error=pthread_create(&idhilo1, NULL, (void *)calculoPi, (void *)(&hilo1_datos));
	if (error != 0)
	{
		perror ("No puedo crear hilo");
		exit (-1);
	}

	error=pthread_create(&idhilo2, NULL, (void *)calculoPi, (void *)(&hilo2_datos));
	if (error != 0)
	{
		perror ("No puedo crear thread");
		exit (-1);
	}
  
	pthread_join(idhilo2, (void **)&valor_devuelto);
	pthread_join(idhilo1, (void **)&valor_devuelto);
  */
  
  double suma = 0.0;
  for (int i = 0; i < CANT_THREADS; i++) {

    suma += threadsData[i].valor;
  }
  printf("%.20f", suma);

	sem_close(semaforo);
	sem_unlink("semaforo_name");
	return 0;
}

