#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_HILOS 16


struct datos{
    int a;
    char b;
};

int carga(){
    double x;
    int i;
    for(i=0; i < 1e09; i++)
        x = x + sin(i);
}

void * funcion(void *datos)
{
    struct datos *datosH;
    datosH = datos;
    printf("\na = %i  b = %c", datosH->a, datosH->b);
    fflush(stdout);
    carga();
}

int main(){

    pthread_t hilo[NUM_HILOS];
    struct datos datosh[NUM_HILOS];
    int r, *rh0, i;
    
    for(i = 0; i < NUM_HILOS; i++)
    {
        datosh[i].a = i;
        datosh[i].b = 'A' + i;
    }
    for(i = 0; i < NUM_HILOS; i++)
    {
        r = pthread_create(&hilo[i], NULL, (void *)funcion, (void *)&datosh[i]);
        if(r != 0)
        {
            perror("\n-->pthread_create error: ");
            exit(-1);
        }
    }
    
    for(i = 0; i < NUM_HILOS; i++)
    {
        r = pthread_join(hilo[i], (void **)&rh0);
        if(r != 0)
        {
            perror("\n-->pthread_join error: ");
            exit(-1);
        }
    }
    printf("\n");
    return 0;    
}