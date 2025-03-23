#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datos.h"

void salir(){
    printf("Saliendo....\n");
    cerrarBD();
    exit(0);

}