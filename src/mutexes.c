/*
    Implementação dos mutexes e das variáveis
*/

#include <pthread.h>
#include <stdio.h>
#include "Matrix.h"
#include "mutexes.h"

static Matrix *Ref;
static Matrix *Y;
static Matrix *Ym;
static Matrix *Ymdot;
// static Matrix* V;
// static Matrix* U;
// static Matrix* X;

static pthread_mutex_t mutexRef     = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexY       = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexYm      = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexYmdot   = PTHREAD_MUTEX_INITIALIZER;

void mutexes_init() {
    Ref     = matrix_zeros(2,1);
    Y       = matrix_zeros(2,1);
    Ym      = matrix_zeros(2,1);
    Ymdot   = matrix_zeros(2,1);
}

void mutexes_destroy() {
    //possivel ponto de falha
    matrix_free(Ref);
    matrix_free(Y);
    matrix_free(Ym);
    matrix_free(Ymdot);


    pthread_mutex_destroy(&mutexRef);
    pthread_mutex_destroy(&mutexY);
    pthread_mutex_destroy(&mutexYm);
    pthread_mutex_destroy(&mutexYmdot);
}

void mutexes_setRef(Matrix* ref) {
    // printf("=>tryset \n");
    pthread_mutex_lock(&mutexRef);
        // printf("===set lock \n");
        // Ref = matrix_copy(ref);
        Ref->ncols = ref->ncols;
        Ref->nlins = ref->nlins;

        for (int i = 0; i < Ref->nlins; i++)
        {
            for (int j = 0; j < Ref->ncols; j++)
            {
                VALUES(Ref, i, j) = matrix_get_value(ref, i, j);
            }
        }
        // * (Ref->values) = *(ref->values);
        // printf("*double values ref : %lf \t *double values REF : %lf \n", *(ref->values), *(Ref->values));
        // printf("ln %d cl %d ln %d cl %d\n", ref->nlins, ref->ncols, Ref->nlins, Ref->ncols);
        pthread_mutex_unlock(&mutexRef);
}

void mutexes_getRef(Matrix *ref) {

    // printf("=>tryget \n");
    if(ref == NULL) return;

    pthread_mutex_lock(&mutexRef);
        // printf("===get lock \n");
    ref->ncols = Ref->ncols;
    ref->nlins = Ref->nlins;

    for (int i = 0; i <  Ref->nlins; i++)
    {
        for (int j = 0; j < Ref->nlins; j++)
        {
            VALUES(ref, i, j) = matrix_get_value(Ref, i, j);
        }
    }
    // *(ref->values)  = * (Ref->values);
    // printf("*double values ref : %lf \t *double values REF : %lf \n", *(ref->values), *(Ref->values));
    // printf("ln %d cl %d ln %d cl %d\n", ref->nlins, ref->ncols, Ref->nlins, Ref->ncols);
    // ref = Ref;
    pthread_mutex_unlock(&mutexRef);
}



void mutexes_setY(Matrix *y) {
    pthread_mutex_lock(&mutexY);
        Y = matrix_copy(y);
        Y->ncols = y->ncols;
        Y->nlins = y->nlins;

        for (int i = 0; i < y->nlins; i++)
        {
            for (int j = 0; j < y->ncols; j++)
            {
                VALUES(Y, i, j) = matrix_get_value(y, i, j);
            }
        }

    pthread_mutex_unlock(&mutexY);
}

void mutexes_getY(Matrix *y) {
    if(y == NULL) return;
    pthread_mutex_lock(&mutexY);
        y->ncols = Y->ncols;
        y->nlins = Y->nlins;

        for (int i = 0; i < y->nlins; i++)
        {
            for (int j = 0; j < y->ncols; j++)
            {
                VALUES(y, i, j) = matrix_get_value(Y, i, j);
            }
        }
    pthread_mutex_unlock(&mutexY);
}

void mutexes_setYm(Matrix  *ym)
{
    pthread_mutex_lock(&mutexYm);
        Ym->ncols = ym->ncols;
        Ym->nlins = ym->nlins;

        for (int i = 0; i < Ym->nlins; i++)
        {
            for (int j = 0; j < Ym->ncols; j++)
            {
                VALUES(Ym, i, j) = matrix_get_value(ym, i, j);
            }
        }
    pthread_mutex_unlock(&mutexYm);
}

void mutexes_getYm(Matrix *ym)
{
    if (ym == NULL) return;
    pthread_mutex_lock(&mutexYm);
        ym->ncols = Ym->ncols;
        ym->nlins = Ym->nlins;

        for (int i = 0; i < ym->nlins; i++)
        {
            for (int j = 0; j < ym->ncols; j++)
            {
                VALUES(ym, i, j) = matrix_get_value(Ym, i, j);
            }
        }
    pthread_mutex_unlock(&mutexYm);
}

void mutexes_setYmdot(Matrix *ymdot)
{
    pthread_mutex_lock(&mutexYmdot);
        Ymdot->ncols = ymdot->ncols;
        Ymdot->nlins = ymdot->nlins;

        for (int i = 0; i < Ymdot->nlins; i++)
        {
            for (int j = 0; j < Ymdot->ncols; j++)
            {
                VALUES(Ymdot, i, j) = matrix_get_value(ymdot, i, j);
            }
        }
    pthread_mutex_unlock(&mutexYmdot);
}

void mutexes_getYmdot(Matrix *ymdot)
{
    if (ymdot) return;
    pthread_mutex_lock(&mutexYmdot);
    ymdot->ncols = Ymdot->ncols;
    ymdot->nlins = Ymdot->nlins;

    for (int i = 0; i < ymdot->nlins; i++)
    {
        for (int j = 0; j < ymdot->ncols; j++)
        {
            VALUES(ymdot, i, j) = matrix_get_value(Ymdot, i, j);
        }
    }
    pthread_mutex_unlock(&mutexYmdot);
}
