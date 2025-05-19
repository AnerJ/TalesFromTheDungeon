#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "personaje.h"
#include "menus.h"
#include "interfaz.h"

void aumentoStats(Clase *p, int sala){
    char promt[10];
    int opcion;
    enviarTexto("Felicidades, valiente aventurero\n");
    sleep(1);

    enviarTexto("Que tu poder crezca y tu determinacion no flaquee\n");
    sleep(1);
    enviarTexto("Elige una de estas tres bonificaciones:\n");
    sleep(1);

    char buffer[256];
    if (sala == 1){
        snprintf(buffer, sizeof(buffer), "1. +2 de Ataque\n2. +2 de Armadura\n3. +2 de Vida\n");
    } else if (sala == 2){
        snprintf(buffer, sizeof(buffer), "1. +3 de Ataque\n2. +1 de Armadura\n3. +5 de Vida\n");
    } else if (sala == 3){
        snprintf(buffer, sizeof(buffer), "1. +10 de Ataque\n2. +3 de Armadura\n3. +10 de Vida\n");
    }
    enviarTexto(buffer);

    enviarTexto("Selecciona una opcion introduciendo el numero: \n");
    getInput(promt, 10);

    if(promt[strlen(promt)-1] == '\n') {
        promt[strlen(promt)-1] = '\0';
    }

    sscanf(promt, "%d", &opcion);

    if (sala == 1){
        if (opcion == 1) p->ataque += 2;
        else if (opcion == 2) p->armadura += 2;
        else if (opcion == 3) p->vida += 2;
        else if (opcion == 4) menuP();
    } else if (sala == 2){
        if (opcion == 1) p->ataque += 3;
        else if (opcion == 2) p->armadura += 1;
        else if (opcion == 3) p->vida += 5;
        else if (opcion == 4) menuP();
    } else if (sala == 3){
        if (opcion == 1) p->ataque += 10;
        else if (opcion == 2) p->armadura += 3;
        else if (opcion == 3) p->vida += 10;
        else if (opcion == 4) menuP();
    }
}
