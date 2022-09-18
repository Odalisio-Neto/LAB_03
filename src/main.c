// Guilherme Matheus - 21950880
// Odalisio Neto - 21950520

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "Parametros.h"
#include "Integral.h"
#include "DString.h"
#include "Matrix.h"

#include "blocos.h"
#include "mutexes.h"
#include "threads.h"



double t = 0;
int contRef=0;
int contModeloRef=0;
int contControle=0;
int contLinearizacao=0;
int contRobo=0;
double T;

double R = 0.3;

//Semaforos para os Produtores
sem_t ProduzRef, ProduzY, ProduzYm, ProduzYmLinha, ProduzV, ProduzX, ProduzU, Print_Mostra;

//Semaforos Para os Consumidores
sem_t ConsomeRef, ConsomeY, ConsomeYm, ConsomeYmLinha, ConsomeV, ConsomeX, ConsomeU;


//Gerar Ref




int main() {
	mutexes_init();

    //Nomeando as Threads
    pthread_t TRef, TModeloRef, TControle, TLinearizacao, TRobo, Print_mostra;

    //Criando as Threads
    pthread_create(&TRef, NULL, Ref, NULL);
    // pthread_create(&TModeloRef, NULL, ModeloRef, NULL);
    // pthread_create(&TControle, NULL, Controle, NULL);
    // pthread_create(&TLinearizacao, NULL, Linear, NULL); 
    // pthread_create(&TRobo, NULL, Robo, NULL);
   // pthread_create(&Print_mostra, NULL, imprimi, NULL);


    //Finalizando as Threads
    pthread_join(TRef, NULL);
    // pthread_join(TModeloRef, NULL);
    // pthread_join(TControle, NULL);
    // pthread_join(TLinearizacao, NULL);
    // pthread_join(TRobo, NULL);

	mutexes_destroy();
    return 0;
}


