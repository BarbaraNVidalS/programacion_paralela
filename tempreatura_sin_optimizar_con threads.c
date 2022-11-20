#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 6 //Cantidad de filas, un thread por fila
#define tam_col 7
#define tam_filas 6
char **matriz_principal [tam_filas][tam_col] = {
            {'C', 'C', 'C', 'C', 'C', 'C', 'C'},
            {'C', 'C', 'C', 'C', 'C', 'C', 'C'},
            {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
            {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
            {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
            {'C', 'C', 'C', 'C', 'C', 'C', 'C'},
        };

//Calculo de heat ecuation
int calculate(int **u){
  int delta_x = 1; 
  //int plate_length = 6;
  int alpha = 2; 
  int delta_t = (pow(delta_x,2))/(4*alpha);
  int gamma = (alpha*delta_t)/(pow(delta_x,2));
  //for k in range(0, max_iter_time-1, 1)
  /*
    for(int i = 1; i < plate_length-1; i++){
        for(int j = 1; j < plate_length-1; j++){
            u[i][j] = gamma*(u[i+1][j] + u[i-1][j] + u[i][j+1] + u[i][j-1] - 4*u[i][j]) + u[i][j];
        }
    }
    */
    for(int i = 1; i < tam_filas; i++){
        for(int j = 1; j < tam_col; j++){
            u[i][j] = gamma*(u[i+1][j] + u[i-1][j] + u[i][j+1] + u[i][j-1] - 4*u[i][j]) + u[i][j];
        }
    }
  return u;
}


void llena_matriz_thread(int **matriz_threads[][7]){ 
    char cold = 'C';
    char hot = 'H';
    for (int y = 0; y < tam_filas; y++) {
        for (int x = 0; x < tam_col; x++) {
            if (matriz_principal[y][x] == cold){
                matriz_threads[y][x] = rand() % 61; //Temperatura entre 0 a 60
            }
            else if (matriz_principal[y][x] == hot){
                matriz_threads[y][x] = rand() % 61 + 40; //Temperatura entre 60 a 100
            }
        }
    }
    return;
}

void imprime_matriz(){
    for (int x = 0; x<tam_filas; x++){
        for (int i = 0; i<tam_col; i++){
            printf("%c ", matriz_principal[x][i]);
        }
        printf("\n");
    }
}

void imprime_matriz_thread(int **matriz_threads[][7]){ //Que imprima bien la matriz con los caracteres
    for (int x = 0; x<tam_filas; x++){
        for (int i = 0; i<tam_col; i++){
            printf("%d ", matriz_threads[x][i]);
            //printf("\n");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) 
{
        pthread_t threads[NUM_THREADS];
        int rc;
        long t;

        int **matriz_threads[tam_filas][tam_col] = {
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0},
        };

        printf("Matriz de temperaturas de caracteres: \n");
        imprime_matriz(matriz_principal);
        printf("\n");

        printf("Matriz de temperatura numerica antes del llenado: \n");
        imprime_matriz_thread(matriz_threads);
        printf("\n");

        //THREAD LLENADO
        pthread_t llenado_matriz;
        rc = pthread_create(&llenado_matriz,NULL,llena_matriz_thread,(char **)matriz_threads);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

        printf("Matriz de temperatura numerica despues del llenado: \n");
        imprime_matriz_thread(matriz_threads);
        
        //HEAT EQUATION 
        int **matriz_heat_ecuation = calculate(matriz_threads);
        printf("Calculo de heat ecuation en matriz: \n");
        imprime_matriz_thread(matriz_heat_ecuation);

    pthread_exit(NULL);
}