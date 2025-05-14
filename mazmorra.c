#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaje.h"
#include "combate.h"
#include "datos.h"
#include "mazmorra.h"
#include "menus.h"
#include <winsock2.h>




void mostrarMapa(const char *nombreFichero, int pos){
    //Aqui se cargara desde un fichero de texto la estructura de la mazmorra y se mostrara en la partida
    //Recibe un integer para saber en que posicion de la mazmorra esta
    FILE *fichero = fopen(nombreFichero, "r");  // Abrir el fichero en modo lectura
    if (!fichero) {
        printf("Error: No se pudo abrir el fichero %s.\n", nombreFichero);
        return;
    }

    char linea[256];  // Buffer para almacenar cada línea del fichero
    while (fgets(linea, sizeof(linea), fichero)) {
        printf("%s", linea);  // Mostrar la línea en la terminal
    }

    fclose(fichero);

    printf("Estas en la sala %i\n", pos);
}



void accionesM(int accion){
    //Esta sera para preguntarle al jugador que acciones querra hacer
    //Por lo general sera solo avanzar y luego saltara el combate por zona
    //Si nos vemos capaces luego metemos la exploracion por sala pero por ahora que funcione lo basico
    //Recibe un int para saber lo que quiere hacer el jugador

    if (accion == 1){
        printf("Avanzas a la siguiente sala...");

    }
    else if (accion == 2){
        printf("Fin del juego, has huido...");
        salir();

    } else if (accion == 4){
        menuP();
    }

}

void mostrarMapaOnline(const char *rutaFichero, int pos, SOCKET sock) {
    FILE *archivo = fopen(rutaFichero, "r");
    if (!archivo) return;

    char linea[256], salida[4096] = "\0";
    int fila = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        if (fila == pos - 1) {
            strcat(salida, "-> ");
        } else {
            strcat(salida, "   ");
        }
        strcat(salida, linea);
        fila++;
    }
    fclose(archivo);
    send(sock, salida, strlen(salida), 0);
}