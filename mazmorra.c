#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaje.h"
#include "combate.h"
#include "datos.h"
#include "mazmorra.h"
#include "salirDelJuego.h"



void mostrarMapa(int pos){
    //Aqui se cargara desde un fichero de texto la estructura de la mazmorra y se mostrara en la partida
    //Recibe un integer para saber en que posicion de la mazmorra esta
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

    }

}