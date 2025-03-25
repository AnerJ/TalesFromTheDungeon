#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "datos.h"

void salir(){
    
    printf("\n=================================\n");
    printf("TALES FROM THE DUNGEON\n");
    printf("=================================\n\n");
    printf("\n\nSaliendo....\n");
    sleep(5);

    cerrarBD();
    exit(0);

}