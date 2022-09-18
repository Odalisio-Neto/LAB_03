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
static pthread_mutex_t mutexXdot       = PTHREAD_MUTEX_INITIALIZER;

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
}

void mutexes_setRef(Matrix* ref) {
    // printf("=>tryset \n");
    pthread_mutex_lock(&mutexRef);
    
        matrix_copy_values(Ref, ref);

    pthread_mutex_unlock(&mutexRef);
}

void mutexes_getRef(Matrix *ref) {

    if(ref == NULL) return;
    pthread_mutex_lock(&mutexRef);
      
        matrix_copy_values(ref, Ref);

    pthread_mutex_unlock(&mutexRef);
}



void mutexes_setY(Matrix *y) {
    pthread_mutex_lock(&mutexY);
    
        matrix_copy_values(Y, y);

    pthread_mutex_unlock(&mutexY);
}

void mutexes_getY(Matrix *y) {
    if(y == NULL) return;
    pthread_mutex_lock(&mutexY);
    
        matrix_copy_values(y, Y);

    pthread_mutex_unlock(&mutexY);
}

void mutexes_setYm(Matrix  *ym)
{
    pthread_mutex_lock(&mutexYm);
    
        matrix_copy_values(Ym, ym);

    pthread_mutex_unlock(&mutexYm);
}

void mutexes_getYm(Matrix *ym)
{
    if (ym == NULL) return;
    pthread_mutex_lock(&mutexYm);
    
        matrix_copy_values(ym, Ym);

    pthread_mutex_unlock(&mutexYm);
}

void mutexes_setYmdot(Matrix *ymdot)
{
    pthread_mutex_lock(&mutexYmdot);
    
        matrix_copy_values(Ymdot, ymdot);

    pthread_mutex_unlock(&mutexYmdot);
}

void mutexes_getYmdot(Matrix *ymdot)
{
    if (ymdot == NULL) return;
    pthread_mutex_lock(&mutexYmdot);

        matrix_copy_values(ymdot, Ymdot);

    pthread_mutex_unlock(&mutexYmdot);
}

void mutexes_setV(Matrix *v)
{
    pthread_mutex_lock(&mutexV);

        matrix_copy_values(V, v);

    pthread_mutex_unlock(&mutexV);
}

void mutexes_getV(Matrix *v)
{
    if (v == NULL) return;
    pthread_mutex_lock(&mutexV);
    
        matrix_copy_values(v, V);

    pthread_mutex_unlock(&mutexV);
}

void mutexes_setU(Matrix *u)
{
    pthread_mutex_lock(&mutexU);

        matrix_copy_values(U, u);

    pthread_mutex_unlock(&mutexU);
}

void mutexes_getU(Matrix *u)
{
    if (u == NULL)
        return;
    pthread_mutex_lock(&mutexU);
        
        matrix_copy_values(u, U);

    pthread_mutex_unlock(&mutexU);
}

void mutexes_setX(Matrix *x)
{
    pthread_mutex_lock(&mutexX);
    
        matrix_copy_values(X, x);

    pthread_mutex_unlock(&mutexX);
}

void mutexes_getX(Matrix *x)
{
    if (x == NULL)
        return;
    pthread_mutex_lock(&mutexX);
    
        matrix_copy_values(x, X);

    pthread_mutex_unlock(&mutexX);
}

void mutexes_setXdot(Matrix *xdot)
{
    pthread_mutex_lock(&mutexXdot);

        matrix_copy_values(Xdot, xdot);
    
    pthread_mutex_unlock(&mutexXdot);
}

void mutexes_getXdot(Matrix *xdot)
{
    if (xdot == NULL)
        return;
    pthread_mutex_lock(&mutexXdot);
    
        matrix_copy_values(xdot, Xdot);

    pthread_mutex_unlock(&mutexXdot);
}
