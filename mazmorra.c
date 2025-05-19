#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaje.h"
#include "combate.h"
#include "datos.h"
#include "mazmorra.h"
#include "menus.h"
#include "interfaz.h"

void mostrarMapa(const char *nombreFichero, int pos){
    FILE *fichero = fopen(nombreFichero, "r");
    if (!fichero) {
        char error[128];
        snprintf(error, sizeof(error), "Error: No se pudo abrir el fichero %s.\n", nombreFichero);
        enviarTexto(error);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), fichero)) {
        enviarTexto(linea);
    }

    fclose(fichero);

    char salaInfo[64];
    snprintf(salaInfo, sizeof(salaInfo), "Estas en la sala %d\n", pos);
    enviarTexto(salaInfo);
}



void accionesM(int accion){
    

    if (accion == 1){
        enviarTexto("Avanzas a la siguiente sala...\n");
    }
    else if (accion == 2){
        enviarTexto("Fin del juego, has huido...\n");
        salir();

    } else if (accion == 4){
        menuP();
    }

}
