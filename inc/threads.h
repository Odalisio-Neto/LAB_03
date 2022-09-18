#include <stdio.h>
#include <math.h>
#include <time.h>

#include "Parametros.h"
#include "Matrix.h"
#include "blocos.h"
#include "mutexes.h"


Matrix *bufferV, *bufferU, *bufferY, *bufferX, *XtLinha, *bufferYmLinha;

void *Ref(void *args);
void *ModeloRef(void *args);
void *Controle(void *args);
void *Linear(void *args);
void *Robo(void *args);



//  // Bloco Controle
