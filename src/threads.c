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
double dt = 0.12;
double R = 0.3;

/**
 * @brief define a thread para Referencia (x_ref, y_ref)
 *
 * @param args
 * @return void*
 */

// TODO jitters e métricas
void *Ref(void *args)
{
    double t = 0;         // tempo calculado
    double tm = 0;        // tempo medido
    double T = TEMPO_REF; // milissegundos
    struct timespec ts1, ts2, ts3 = {0};

    Matrix *bufferRef = matrix_zeros(2, 1);
    Matrix *testBuf = matrix_zeros(2, 1);

    while (t <= TEMPO_MAX)
    {

        clock_gettime(CLOCK_REALTIME, &ts1);

        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + T;

       

        bufferRef = defineRef(t / 1000); // verificar t = t*T ()
        mutexes_setRef(bufferRef);
        printf("%lf,%lf,%lf\n", 
            t, 
            matrix_get_value(bufferRef, 0, 0)     , matrix_get_value(bufferRef, 1, 0));


        mutexes_getRef(testBuf);

        // printf("x_ref : %.4f y_ref : %.4f\n", VALUES(testBuf, 0, 0), VALUES(testBuf, 1, 0));
        // printf("%.4f, %.4f, t: %lf, ts1 : %ld , TMAX: %d \n",
        //        VALUES(bufferRef, 0, 0), VALUES(bufferRef, 1, 0), t/1000, ts1.tv_nsec, TEMPO_MAX * 1000);
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

// TODO jitters
void *ModeloRef(void *args)
{
    // clock_t start;
    double t = 0;  // tempo calculado
    double tm = 0; // tempo medido
    struct timespec ts1, ts2, ts3 = {0};

    T = TEMPO_MODELO_REF;
    Matrix *bufferRef = matrix_zeros(2, 1);
    Matrix *bufferYm = matrix_zeros(2, 1);
    Matrix *bufferYmdot = matrix_zeros(2, 1);
    Matrix *bufferYmdot_old = matrix_zeros(2, 1);

    while (t <= TEMPO_MAX)
    {
        clock_gettime(CLOCK_REALTIME, &ts1);
        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + T;

        mutexes_getRef(bufferRef);
        mutexes_getYm(bufferYm);
        mutexes_getYmdot(bufferYmdot_old);

        bufferYmdot = y_m(bufferRef, bufferYm);
        bufferYm = ModeloRefYm(bufferYmdot, bufferYmdot_old, dt);

        // printf("%.4f, %.4f,  %.4f, %.4f \n", VALUES(bufferYm, 0, 0),
        //        VALUES(bufferYm, 1, 0), VALUES(bufferYmdot, 0, 0), VALUES(bufferYmdot, 1, 0));



        mutexes_setYm(bufferYm);
        mutexes_setYmdot(bufferYmdot);

        // JitterModeloRef[contModeloRef]=TEMPO__MODELO_REF - dif/1000.0;

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);


        nanosleep(&ts3, &ts3);
        contModeloRef++;
    }
    
    matrix_free(bufferYmdot);
    matrix_free(bufferYm);
    matrix_free(bufferRef);

    return 0;
}

//  // Bloco Controle
 void *Controle(void *args)
 {
    struct timespec ts1, ts2, ts3 = {0};
    double tm = 0; // tempo medido
    double t = 0;

    T = TEMPO_CONTROLE;

    Matrix *bufferV = matrix_zeros(2, 1);
    Matrix *bufferY = matrix_zeros(2, 1);
    Matrix *bufferYm = matrix_zeros(2, 1);
    Matrix *bufferYmdot = matrix_zeros(2, 1);
    while (t <= TEMPO_MAX)
    {
        // Y, Ym, Ymdot, V

        clock_gettime(CLOCK_REALTIME, &ts1);
        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + T;

        mutexes_getY(bufferY);
        mutexes_getYm(bufferYm);
        mutexes_getYmdot(bufferYmdot);

        bufferV = ControleBloco(bufferYmdot, bufferYm, bufferY);

        mutexes_setV(bufferV);
        // TODO verificar se Y está com saida depois de implementar o bloco Y
        // printf("V(0,0) : %.4f V(1,0) : %.4f, y(0,0) : %.4f y(1,0) : %.4f, y_m(0,0) : %.4f y_mdot(0,0) : %.4f, t: %lf \n",
        //        VALUES(bufferV, 0, 0), VALUES(bufferV, 1, 0), VALUES(bufferY, 0, 0), VALUES(bufferY, 1, 0),
        //         VALUES(bufferYm, 0, 0), VALUES(bufferYmdot, 0, 0), t / 1000);

        mutexes_setYm(bufferYm);
        mutexes_setYmdot(bufferYmdot);

        // JitterModeloRef[contModeloRef]=TEMPO__MODELO_REF - dif/1000.0;

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);

        nanosleep(&ts3, &ts3);
        contModeloRef++;
        // JitterControle[contControle]=TEMPO_CONTROLE-dif/1000.0;
    }
    matrix_free(bufferYmdot);
    matrix_free(bufferYm);
    matrix_free(bufferV);

    return 0;
}

void *Linear(void *args)
{
    struct timespec ts1, ts2, ts3 = {0};
    double tm = 0; // tempo medido
    double t = 0;

    T = TEMPO_LINEARIZACAO;

    Matrix *bufferU = matrix_zeros(2, 1);
    Matrix *bufferV = matrix_zeros(2, 1);
    Matrix *bufferX = matrix_zeros(2, 1);

    while (t <= TEMPO_MAX)
    {  
        //X, V, U
        clock_gettime(CLOCK_REALTIME, &ts1);
        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + TEMPO_LINEARIZACAO;

        // mutexes_getU(bufferU);
        mutexes_getX(bufferX);
        mutexes_getV(bufferV);

        bufferU = Linearizacao(bufferX, bufferV, R);

        // printf("%4lf %4lf \n", VALUES(bufferU, 0, 0), VALUES(bufferU, 1, 0));
        mutexes_setU(bufferU);

        // JitterLinearizacao[contLinearizacao]=TEMPO_LINEARIZACAO -dif/1000.0;
        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = TEMPO_LINEARIZACAO * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
        contLinearizacao++;
    }
    matrix_free(bufferU);
    matrix_free(bufferV);
    matrix_free(bufferX);

    return 0;
}


void *Robo(void *args)
{
    struct timespec ts1, ts2, ts3 = {0};
    double tm = 0;
    double t = 0;
    T = TEMPO_ROBO;

    Matrix *bufferY =  matrix_zeros(2, 1);
    Matrix *bufferU = matrix_zeros(2, 1);
    Matrix *bufferV = matrix_zeros(2, 1);

    Matrix *bufferX = matrix_zeros(3, 1);
    Matrix *bufferXdot = matrix_zeros(3, 1);
    Matrix *bufferXdot_old = matrix_zeros(3, 1);

    while (t <= TEMPO_MAX)
    {
        //U X Y
        clock_gettime(CLOCK_REALTIME, &ts1);
        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + T;


        // bufferX = RoboXt(bufferXdot, bufferX, dt);
        
        mutexes_getU(bufferU);
        mutexes_getV(bufferV);
        mutexes_getX(bufferX);
        mutexes_getY(bufferY);
        mutexes_getXdot(bufferXdot_old);
        // mutexes_getXdot(bufferXdot);


        bufferXdot = RoboXtdot(bufferX, bufferU);
        bufferX = RoboXt(bufferXdot, bufferXdot_old, 0.012);
        bufferY = RoboYt(bufferX, R);

        mutexes_setXdot(bufferXdot);
        mutexes_setX(bufferX);
        mutexes_setY(bufferY);

        // JitterRobo[contRobo]=TEMPO_ROBO - dif/1000.0;

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T * 1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
        contRobo++;
    }
    matrix_free(bufferXdot);
    // matrix_free(bufferXdot_old);
    matrix_free(bufferU);
    matrix_free(bufferV);
    matrix_free(bufferX);
    matrix_free(bufferY);
    return 0;
}

void *print_thread(void *args) {
    double t = 0;       //tempo calculado
    double tm = 0;      //tempo medido
    double T = 10;      //milissegundos
    struct timespec ts1, ts2, ts3 = {0};
    
    Matrix* Ref      = matrix_zeros(2,1);
    Matrix* Y        = matrix_zeros(2,1);
    Matrix* Ym       = matrix_zeros(2,1);
    Matrix* Ymponto  = matrix_zeros(2,1);
    Matrix* V        = matrix_zeros(2,1);
    Matrix* U        = matrix_zeros(2,1);
    Matrix* X        = matrix_zeros(3,1);


    while(t <= TEMPO_MAX) {
        clock_gettime(CLOCK_REALTIME, &ts1);
        tm = 1000000 * ts1.tv_nsec - tm;
        t = t + T;

        mutexes_lockPrint();

        mutexes_getRef(Ref);
        mutexes_getYmdot(Ymponto);
        mutexes_getYm(Ym);
        mutexes_getV(V);
        mutexes_getU(U);
        mutexes_getX(X);
        mutexes_getY(Y);

        // printf("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", 
        //     t, 
        //     matrix_get_value(Ref, 0, 0)     , matrix_get_value(Ref, 1, 0),
        //     matrix_get_value(Ymponto, 0, 0) , matrix_get_value(Ymponto, 1, 0),
        //     matrix_get_value(Ym, 0, 0)      , matrix_get_value(Ym, 1, 0),
        //     matrix_get_value(V, 0, 0)       , matrix_get_value(V, 1, 0),
        //     matrix_get_value(U, 0, 0)       , matrix_get_value(U, 1, 0),
        //     matrix_get_value(X, 0, 0)       , matrix_get_value(X, 1, 0), matrix_get_value(X, 2, 0),
        //     matrix_get_value(Y, 0, 0)       , matrix_get_value(Y, 1, 0)
        // );

        printf("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
               t,
               matrix_get_value(Ref, 0, 0),     // X_ref
               matrix_get_value(Ref, 1, 0),     // Y_ref
               matrix_get_value(Ymponto, 0, 0), // Ydot_x
               matrix_get_value(Ymponto, 1, 0), // Ydot_x
               matrix_get_value(Ym, 0, 0),      // Ymx
               matrix_get_value(Ym, 1, 0),      // Ymy
               matrix_get_value(V, 0, 0),       // Vx
               matrix_get_value(V, 1, 0),       // Vy
               matrix_get_value(U, 0, 0),       // Ux
               matrix_get_value(U, 1, 0),       // Uy
               matrix_get_value(X, 0, 0),       // X1
               matrix_get_value(X, 1, 0),       // X2
               matrix_get_value(X, 2, 0),       // X3
               matrix_get_value(Y, 0, 0),       // Y1
               matrix_get_value(Y, 1, 0)        // Y2
        );

        mutexes_unlockPrint();

        clock_gettime(CLOCK_REALTIME, &ts2);
        ts3.tv_sec = 0;
        ts3.tv_nsec = T*1000000 - (ts2.tv_nsec - ts1.tv_nsec);
        nanosleep(&ts3, &ts3);
    }

    matrix_free(Ref);
    matrix_free(Y);
    matrix_free(Ym);
    matrix_free(Ymponto);
    matrix_free(V);
    matrix_free(U);
    matrix_free(X);

    return 0;
}
