#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "personaje.h"
#include "menus.h"
#include "datos.h"
#include "interfaz.h"  // Para enviarTexto y getInput

Clase * crearPersonaje(){
    Clase * personaje = NULL;
    personaje = (Clase*) malloc (sizeof(Clase));

    enviarTexto("\n===============\n");
    enviarTexto("Creacion de Personaje\n");
    enviarTexto("===============\n");
    sleep(2);
    enviarTexto("\nDime tu nombre valiente aventurero: \n");

    char nombre[10];
    getInput(nombre, sizeof(nombre));

    if (nombre[strlen(nombre)-1] == '\n') {
        nombre[strlen(nombre)-1] = '\0';
    }

    enviarTexto("Valiente aventurero ");
    enviarTexto(nombre);
    enviarTexto(", Elige tu clase: \n");


    strcpy(personaje->nombre, nombre);

    enviarTexto("\n*****************\n");
    enviarTexto("| 1. Guerrero   |\n");
    enviarTexto("| Ataque: 1d12  |\n");
    enviarTexto("| Armadura: 7   |\n");
    enviarTexto("| Vida: 20      |\n");
    enviarTexto("| Velocidad: 2  |\n");
    enviarTexto("*****************\n");
    sleep(2);

    enviarTexto("\n*****************\n");
    enviarTexto("| 2. Cazador    |\n");
    enviarTexto("| Ataque: 2d8   |\n");
    enviarTexto("| Armadura: 5   |\n");
    enviarTexto("| Vida: 15      |\n");
    enviarTexto("| Velocidad: 5  |\n");
    enviarTexto("*****************\n");
    sleep(2);

    enviarTexto("\n*****************\n");
    enviarTexto("| 3. Picaro     |\n");
    enviarTexto("| Ataque: 3d5   |\n");
    enviarTexto("| Armadura: 2   |\n");
    enviarTexto("| Vida: 13      |\n");
    enviarTexto("| Velocidad: 8  |\n");
    enviarTexto("*****************\n");
    sleep(2);

    enviarTexto("\nElige una clase: \n");

    char promt2[10];
    int opcion2;
    getInput(promt2, 10);

    if (promt2[strlen(promt2)-1] == '\n') {
        promt2[strlen(promt2)-1] = '\0';
    }

    sscanf(promt2, "%d", &opcion2);

    if (opcion2 == 1) {
        enviarTexto("\nHas elegido el Guerrero\n\n");
        cargarClase(opcion2, personaje);

    } else if (opcion2 == 2) {
        enviarTexto("\nHas elegido el Cazador\n");
        cargarClase(opcion2, personaje);

    } else if (opcion2 == 3) {
        enviarTexto("\nHas elegido el Picaro\n");
        cargarClase(opcion2, personaje);

    } else if (opcion2 == 4) {
        menuP();

    } else if (opcion2 == 5) {
        salir();
    }

    personaje->pos = 0;
    personaje->idJugador = insertarJugador(nombre, opcion2);

    return personaje;
}
