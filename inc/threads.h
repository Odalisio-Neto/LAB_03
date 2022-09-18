#include <stdio.h>
#include <math.h>
#include <time.h>

#include "Parametros.h"
#include "Matrix.h"
#include "blocos.h"
#include "mutexes.h"

double t = 0;
int contRef = 0;
int contModeloRef = 0;
int contControle = 0;
int contLinearizacao = 0;
int contRobo = 0;
double T;

double R = 0.3;

/**
 * @brief define a thread para Referencia (x_ref, y_ref) 
 * 
 * @param args 
 * @return void* 
 */

void *Ref(void *args)
{
    double t = 0;         // tempo calculado
    double tm = 0;        // tempo medido
    double T = TEMPO_REF; // milissegundos
    struct timespec ts1, ts2, ts3 = {0};
    Matrix *bufferRef = matrix_zeros(2, 1);

    while (t <= TEMPO_MAX * 1000)
    {

        clock_gettime(CLOCK_REALTIME, &ts1);

        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + T;


        bufferRef = defineRef(t/1000); // verificar t = t*T ()
        mutexes_setRef(bufferRef);
        printf("x_ref : %.4f y_ref : %.4f, t: %lf, ts1 : %ld , TMAX: %d \n",
               VALUES(bufferRef, 0, 0), VALUES(bufferRef, 1, 0), t/1000, ts1.tv_nsec, TEMPO_MAX * 1000);
        // JitterRef[contRef]=TEMPO_REF - dif/1000.0;
        contRef++;

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }
    matrix_free(bufferRef);

    return 0;
}

Matrix *bufferV, *bufferU, *bufferY, *bufferX, *XtLinha, *bufferYm, *bufferYmLinha, *auxBuffer, *auxBuffer2;

//TODO demais threads

// void *ModeloRef(void *args)
// {
//     // clock_t start;
//     struct timespec ts1, ts2, ts3 = {0};
//     double dif;
//     double tm;
//     T = TEMPO_MODELO_REF;
//     Matrix *bufferRef;

//     while (t <= TEMPO_MAX)
//     {
//         // start=clock();
//         sem_wait(&ConsomeRef);
//         sem_wait(&ProduzYm);
//         sem_wait(&ProduzYmLinha);
//         clock_gettime(CLOCK_REALTIME, &ts1);
//         tm = 1000000 * ts1.tv_nsec - tm;
//         t = t + T;
//         auxBuffer = matrix_copy(bufferYmLinha);
//         matrix_free(bufferYmLinha);

//         mutexes_getRef(bufferRef);

//         bufferYmLinha = y_m(bufferRef, bufferYm);
//         bufferYm = ModeloRefYm(bufferYmLinha, auxBuffer, t * T);

//         sem_post(&ProduzRef);
//         sem_post(&ConsomeYm);
//         sem_post(&ConsomeYmLinha);

//         dif = t - tm;
//         // JitterModeloRef[contModeloRef]=TEMPO__MODELO_REF - dif/1000.0;
//         // usleep(TEMPO_MODELO_REF_MS - dif);

//         clock_gettime(CLOCK_REALTIME, &ts2);
//         ts3.tv_sec = 0;
//         ts3.tv_nsec = T * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);
//         nanosleep(&ts3, &ts3);
//         contModeloRef++;
//     }
// }

// // Bloco Controle
// void *Controle(void *args)
// {
//     struct timespec ts1, ts2, ts3 = {0};
//     double dif;
//     double tm;
//     T = TEMPO_CONTROLE;

//     while (t <= TEMPO_MAX)
//     {
//         // start=clock();
//         sem_wait(&ConsomeY);
//         sem_wait(&ConsomeYm);
//         sem_wait(&ConsomeYmLinha);
//         sem_wait(&ProduzV);
//         clock_gettime(CLOCK_REALTIME, &ts1);
//         tm = 1000000 * ts1.tv_nsec - tm;
//         t = t + T;

//         matrix_free(bufferV);
//         bufferV = ControleBloco(bufferYmLinha, bufferYm, bufferY);
//         sem_post(&ProduzYm);
//         sem_post(&ProduzYmLinha);
//         sem_post(&ProduzY);
//         sem_post(&ConsomeV);
//         dif = t - tm;
//         // JitterControle[contControle]=TEMPO_CONTROLE-dif/1000.0;
//         // usleep(TEMPO_CONTROLE_MS-dif);
//     }
// }

// void *Linear(void *args)
// {
//     struct timespec ts1, ts2, ts3 = {0};
//     double dif;
//     double tm;
//     T = TEMPO_LINEARIZACAO;
//     while (t <= TEMPO_MAX)
//     {
//         sem_wait(&ConsomeX);
//         sem_wait(&ConsomeV);
//         sem_wait(&ProduzU);
//         clock_gettime(CLOCK_REALTIME, &ts1);
//         tm = 1000000 * ts1.tv_nsec - tm;
//         t = t + TEMPO_LINEARIZACAO;

//         matrix_free(bufferU);
//         bufferU = Linearizacao(bufferX, bufferV, R);
//         sem_post(&ProduzX);
//         sem_post(&ProduzV);
//         sem_post(&ConsomeU);
//         dif = t - tm;
//         // JitterLinearizacao[contLinearizacao]=TEMPO_LINEARIZACAO -dif/1000.0;
//         clock_gettime(CLOCK_REALTIME, &ts2);
//         ts3.tv_sec = 0;
//         ts3.tv_nsec = TEMPO_LINEARIZACAO * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);
//         nanosleep(&ts3, &ts3);
//         contLinearizacao++;
//     }
// }

// void *Robo(void *args)
// {
//     struct timespec ts1, ts2, ts3 = {0};
//     double dif;
//     double tm;
//     T = TEMPO_ROBO;
//     while (t <= TEMPO_MAX)
//     {
//         sem_wait(&ConsomeU);
//         sem_wait(&ProduzX);
//         sem_wait(&ProduzY);
//         clock_gettime(CLOCK_REALTIME, &ts1);
//         tm = 1000000 * ts1.tv_nsec - tm;
//         t = t + T;

//         printf("%.2lf, %lf, %lf, %lf\n", t - 0.12,
//                matrix_get_value(bufferX, 0, 0),
//                matrix_get_value(bufferX, 1, 0),
//                matrix_get_value(bufferX, 2, 0));

//         auxBuffer2 = matrix_copy(XtLinha);

//         matrix_free(XtLinha);
//         XtLinha = RoboXtLinha(bufferX, bufferU);

//         matrix_free(bufferX);
//         bufferX = RoboXt(XtLinha, auxBuffer, t * T);

//         matrix_free(bufferY);
//         bufferY = RoboYt(bufferX, R);
//         sem_post(&ProduzU);
//         sem_post(&ConsomeX);
//         sem_post(&ConsomeY);
//         // JitterRobo[contRobo]=TEMPO_ROBO - dif/1000.0;

//         clock_gettime(CLOCK_REALTIME, &ts2);
//         ts3.tv_sec = 0;
//         ts3.tv_nsec = T * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);
//         nanosleep(&ts3, &ts3);
//         contRobo++;
//     }
// }
