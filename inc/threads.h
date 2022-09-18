#include <stdio.h>
#include <math.h>
#include <time.h>

#include "Parametros.h"
#include "Matrix.h"
#include "blocos.h"
#include "mutexes.h"



void *Ref(void *args);

Matrix *bufferV, *bufferU, *bufferY, *bufferX, *XtLinha, *bufferYmLinha;

void *ModeloRef(void *args);

// TODO demais threads
//  // Bloco Controle
//  void *Controle(void *args)
//  {
//      struct timespec ts1, ts2, ts3 = {0};
//      double dif;
//      double tm;
//      T = TEMPO_CONTROLE;

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
