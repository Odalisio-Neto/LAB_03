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
static Matrix* V;
static Matrix* U;
static Matrix* X;
static Matrix* Xdot;

static pthread_mutex_t mutexRef     = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexY       = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexYm      = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexYmdot   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexV       = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexU       = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexX       = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexXdot    = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t mutexprint   = PTHREAD_MUTEX_INITIALIZER;

void mutexes_init() {
    Ref     = matrix_zeros(2,1);
    Y       = matrix_zeros(2,1);
    Ym      = matrix_zeros(2,1);
    Ymdot   = matrix_zeros(2,1);
    V = matrix_zeros(2, 1);
    U = matrix_zeros(2, 1);
    X = matrix_zeros(3, 1);
    Xdot = matrix_zeros(3, 1);
}

void mutexes_destroy() {
    //possivel ponto de falha
    matrix_free(Ref);
    matrix_free(Y);
    matrix_free(Ym);
    matrix_free(Ymdot);
    matrix_free(V);
    matrix_free(U);
    matrix_free(X);
    matrix_free(Xdot);

    pthread_mutex_destroy(&mutexRef);
    pthread_mutex_destroy(&mutexY);
    pthread_mutex_destroy(&mutexYm);
    pthread_mutex_destroy(&mutexYmdot);
    pthread_mutex_destroy(&mutexV);
    pthread_mutex_destroy(&mutexU);
    pthread_mutex_destroy(&mutexX);
    pthread_mutex_destroy(&mutexXdot);

    pthread_mutex_destroy(&mutexprint);
}

void mutexes_lockPrint() {
    pthread_mutex_lock(&mutexprint);
}

void mutexes_unlockPrint() {
    pthread_mutex_unlock(&mutexprint);
}

void mutexes_setRef(Matrix *ref)
{
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
            VALUES(Ref, i, j) = VALUES(ref, i, j);
        }
    }
    // * (Ref->values) = *(ref->values);
    // printf("*double values ref : %lf \t *double values REF : %lf \n", *(ref->values), *(Ref->values));
    // printf("ln %d cl %d ln %d cl %d\n", ref->nlins, ref->ncols, Ref->nlins, Ref->ncols);
    pthread_mutex_unlock(&mutexRef);
}

void mutexes_getRef(Matrix *ref)
{

    // printf("=>tryget \n");
    if (ref == NULL)
        return;

    pthread_mutex_lock(&mutexRef);
    // printf("===get lock \n");
    // ref->ncols = Ref->ncols;
    // ref->nlins = Ref->nlins;

    for (int i = 0; i < Ref->nlins; i++)
    {
        for (int j = 0; j < Ref->nlins; j++)
        {
            VALUES(ref, i, j) = VALUES(Ref, i, j);
        }
    }
    // ref = Ref;
    // ref->values = Ref->values;

    // *(ref->values)  = * (Ref->values);
    // printf("*double values ref : %lf \t *double values REF : %lf \n", *(ref->values), *(Ref->values));
    // printf("ln %d cl %d ln %d cl %d\n", ref->nlins, ref->ncols, Ref->nlins, Ref->ncols);
    // ref = Ref;
    pthread_mutex_unlock(&mutexRef);
}

void mutexes_setY(Matrix *y)
{
    pthread_mutex_lock(&mutexY);
    Y = matrix_copy(y);
    Y->ncols = y->ncols;
    Y->nlins = y->nlins;

    for (int i = 0; i < y->nlins; i++)
    {
        for (int j = 0; j < y->ncols; j++)
        {
            VALUES(Y, i, j) = VALUES(y, i, j);
        }
    }

    pthread_mutex_unlock(&mutexY);
}

void mutexes_getY(Matrix *y)
{
    if (y == NULL)
        return;
    pthread_mutex_lock(&mutexY);
    y->ncols = Y->ncols;
    y->nlins = Y->nlins;

    for (int i = 0; i < y->nlins; i++)
    {
        for (int j = 0; j < y->ncols; j++)
        {
            VALUES(y, i, j) = VALUES(Y, i, j);
        }
    }
    pthread_mutex_unlock(&mutexY);
}

void mutexes_setYm(Matrix *ym)
{
    pthread_mutex_lock(&mutexYm);
    Ym->ncols = ym->ncols;
    Ym->nlins = ym->nlins;

    for (int i = 0; i < Ym->nlins; i++)
    {
        for (int j = 0; j < Ym->ncols; j++)
        {
            VALUES(Ym, i, j) = VALUES(ym, i, j);
        }
    }
    pthread_mutex_unlock(&mutexYm);
}

void mutexes_getYm(Matrix *ym)
{
    if (ym == NULL)
        return;
    pthread_mutex_lock(&mutexYm);
    ym->ncols = Ym->ncols;
    ym->nlins = Ym->nlins;

    for (int i = 0; i < ym->nlins; i++)
    {
        for (int j = 0; j < ym->ncols; j++)
        {
            VALUES(ym, i, j) = VALUES(Ym, i, j);
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
            VALUES(Ymdot, i, j) = VALUES(ymdot, i, j);
        }
    }
    pthread_mutex_unlock(&mutexYmdot);
}

void mutexes_getYmdot(Matrix *ymdot)
{
    if (ymdot == NULL)
        return;
    pthread_mutex_lock(&mutexYmdot);
    ymdot->ncols = Ymdot->ncols;
    ymdot->nlins = Ymdot->nlins;

    for (int i = 0; i < ymdot->nlins; i++)
    {
        for (int j = 0; j < ymdot->ncols; j++)
        {
            VALUES(ymdot, i, j) = VALUES(Ymdot, i, j);
        }
    }
    pthread_mutex_unlock(&mutexYmdot);
}

void mutexes_setV(Matrix *v)
{
    pthread_mutex_lock(&mutexV);
    V->ncols = v->ncols;
    V->nlins = v->nlins;
    for (int i = 0; i < v->nlins; i++)
    {
        for (int j = 0; j < v->ncols; j++)
        {
            VALUES(V, i, j) = VALUES(v, i, j);
        }
    }
    pthread_mutex_unlock(&mutexV);
}

void mutexes_getV(Matrix *v)
{
    if (v == NULL)
        return;
    pthread_mutex_lock(&mutexV);
    v->ncols = V->ncols;
    v->nlins = V->nlins;

    for (int i = 0; i < v->nlins; i++)
    {
        for (int j = 0; j < v->ncols; j++)
        {
            VALUES(v, i, j) = VALUES(V, i, j);
        }
    }
    pthread_mutex_unlock(&mutexV);
}

void mutexes_setU(Matrix *u)
{
    pthread_mutex_lock(&mutexU);
    U->ncols = u->ncols;
    U->nlins = u->nlins;

    for (int i = 0; i < U->nlins; i++)
    {
        for (int j = 0; j < U->ncols; j++)
        {
            VALUES(U, i, j) = VALUES(u, i, j);
        }
    }
    pthread_mutex_unlock(&mutexU);
}

void mutexes_getU(Matrix *u)
{
    if (u == NULL)
        return;
    pthread_mutex_lock(&mutexU);
    u->ncols = U->ncols;
    u->nlins = U->nlins;

    for (int i = 0; i < u->nlins; i++)
    {
        for (int j = 0; j < u->ncols; j++)
        {
            VALUES(u, i, j) = VALUES(U, i, j);
        }
    }
    pthread_mutex_unlock(&mutexU);
}

void mutexes_setX(Matrix *x)
{
    pthread_mutex_lock(&mutexX);
    X->ncols = x->ncols;
    X->nlins = x->nlins;

    for (int i = 0; i < X->nlins; i++)
    {
        for (int j = 0; j < X->ncols; j++)
        {
            VALUES(X, i, j) = VALUES(x, i, j);
        }
    }
    pthread_mutex_unlock(&mutexX);
}

void mutexes_getX(Matrix *x)
{
    if (x == NULL)
        return;
    pthread_mutex_lock(&mutexX);
    x->ncols = X->ncols;
    x->nlins = X->nlins;

    for (int i = 0; i < x->nlins; i++)
    {
        for (int j = 0; j < x->ncols; j++)
        {
            VALUES(x, i, j) = VALUES(X, i, j);
        }
    }
    pthread_mutex_unlock(&mutexX);
}


void mutexes_setXdot(Matrix *xdot)
{
    pthread_mutex_lock(&mutexXdot);

    if (xdot == NULL)
        return;
    Xdot->ncols = xdot->ncols;
    Xdot->nlins = xdot->nlins;

    for (int i = 0; i < xdot->nlins; i++)
    {
        for (int j = 0; j < xdot->ncols; j++)
        {
            VALUES(Xdot, i, j) = VALUES(xdot, i, j);
        }
    }

    pthread_mutex_unlock(&mutexXdot);
}

void mutexes_getXdot(Matrix *xdot)
{
    if (xdot == NULL)
        return;
    pthread_mutex_lock(&mutexXdot);

    xdot->ncols = Xdot->ncols;
    xdot->nlins = Xdot->nlins;

    for (int i = 0; i < xdot->nlins; i++)
    {
        for (int j = 0; j < xdot->ncols; j++)
        {
            VALUES(xdot, i, j) = VALUES(Xdot, i, j);
        }
    }

    pthread_mutex_unlock(&mutexXdot);
}
