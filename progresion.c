#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "personaje.h"
#include "salirDelJuego.h"
#include "progresion.h"




void aumentoStats(Clase *p, int sala){
    char promt[10];
    int opcion;
    printf("Felicidades, valiente aventurero\n" );
    sleep(1);

    printf("Que tu poder crezca y tu determinacion no flaquee\n");
    sleep(1);
    printf("Elige una de estas tres bonificaciones:\n ");
    sleep(1);
    
    if (sala == 1){
        printf("1. +2 de Ataque\n 2. +2 de Armadura\n 3. +2 de Vida\n");
        
    } else if (sala == 2){
        printf("1. +3 de Ataque \n 2. +1 de Armadura \n 3. +5 de Vida\n");

    } else if (sala == 3){
        printf("1. +10 de Ataque \n 2. +3 de Armadura \n 3. +10 de Vida\n");

    }




    printf("Selecciona una opcion introduciendo el numero: \n");
    fgets(promt, 10, stdin);

        //Eliminar el salto de linea
        if( promt[strlen(promt)-1] == '\n'){
            promt[strlen(promt)-1] = '\0';
        }

        //Pasar el valor a entero
    sscanf(promt, "%d", &opcion );


    if (sala == 1){
        if (opcion == 1){
            p->ataque += 2;

        } else if (opcion == 2){
            p->armadura+= 2;

        } else if (opcion == 3){
            p->vida += 2;
        }



    } else if (sala == 2){
        if (opcion == 1){
            p->ataque += 3;

        } else if (opcion == 2){
            p->armadura+= 1;

        } else if (opcion == 3){
            p->vida += 5;
        }

    } else if (sala == 3){
        if (opcion == 1){
            p->ataque += 10;

        } else if (opcion == 2){
            p->armadura+= 3;

        } else if (opcion == 3){
            p->vida += 10;
        }


    }


}


int guardarGameState(const GameState *state) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s",
             state->idJugador,
             state->nombreJugador,
             state->estadisticas.vida,
             state->estadisticas.armadura,
             state->estadisticas.velocidad,
             state->estadisticas.ataque,
             state->estadisticas.veces,
             state->nombreJugador,
             state->salaActual,
             state->enemigosEliminados,
             state->enemigosRestantes);

    return guardarPartida(state->idJugador, buffer);
}