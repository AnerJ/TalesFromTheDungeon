#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "personaje.h"
#include "menus.h"
#include "combate.h"
#include "datos.h"
#include "mazmorra.h"
#include "progresion.h"
#include "sqlite3.h"
#include "personaje.h"
#include "interfaz.h"  // Redirección de I/O

void salir() {
    enviarTexto("\n=================================\n");
    enviarTexto("TALES FROM THE DUNGEON\n");
    enviarTexto("=================================\n\n");
    enviarTexto("\n\nSaliendo....\n");
    sleep(5);
    cerrarBD();
    exit(0);
}

void menuP() {
    inicializarBD();
    insertarClases();
    insertarEnemigos();


    char promt[10];
    int opcion;
    do {
        mostrarMenu();
        enviarTexto("Selecciona una opcion introduciendo el numero: \n");
        getInput(promt, 10);

        if (promt[strlen(promt)-1] == '\n') {
            promt[strlen(promt)-1] = '\0';
        }

        sscanf(promt, "%d", &opcion);

        if (opcion == 1) {
            nuevaPartida();
            break;

        } else if (opcion == 2) {
            cargarPartida();
            break;

        } else if (opcion == 3) {
            salir();
        }

    } while (opcion != 3);
}

void mostrarMenu() {
    enviarTexto("\n=================================\n");
    enviarTexto("TALES FROM THE DUNGEON\n");
    enviarTexto("=================================\n");
    sleep(3);
    enviarTexto("1. Nueva Partida\n");
    enviarTexto("2. Cargar Partida\n");
    enviarTexto("3. Salir\n");
}

void nuevaPartida() {
    enviarTexto("\n=================================\n");
    enviarTexto("TALES FROM THE DUNGEON\n");
    enviarTexto("=================================\n\n");
    enviarTexto("Creando nueva partida... \n");
    sleep(1);

    Clase *clase = crearPersonaje();
    if (clase == NULL) {
        enviarTexto("Error al crear personaje\n");
    }
    inciarPartida(clase);
}

void cargarPartida() {
    enviarTexto("Cargando partida... \n");
    sleep(4);

    Clase *claseC = (Clase *)malloc(sizeof(Clase));
    mostrarPartidasGuardadas();
    char nombreP[10];
    int eleccion;

    enviarTexto("Elige la partida que quieres cargar introduciendo el numero: ");
    getInput(nombreP, 10);

    if (nombreP[strlen(nombreP)-1] == '\n') {
        nombreP[strlen(nombreP)-1] = '\0';
    }

    sscanf(nombreP, "%d", &eleccion);

    partidasCargadas(eleccion, claseC);
    enviarTexto("Datos Cargados\n");
    inciarPartida(claseC);
}

void inciarPartida(Clase *p) {
    int cantidadDeEnemigos = 4;
    Enemigo enemigos[4];
    cargarEnemigos(enemigos, cantidadDeEnemigos);
    char txt[10];
    int accion;

    enviarTexto("\nTe adentras a la mazmorra...\n");
    sleep(2);

    while (p->vida > 0) {
        mostrarMapa("ficheros/mazmorraMapa.txt", p->pos + 1);

        enviarTexto("Cual es tu siguiente accion: \n1. Avanzar\n2. Huir\n");
        getInput(txt, 10);

        if (txt[strlen(txt)-1] == '\n') {
            txt[strlen(txt)-1] = '\0';
        }

        sscanf(txt, "%d", &accion);

        if (accion == 5) {
            salir();
        }

        accionesM(accion);
        Enemigo e = enemigos[p->pos];
        iniciarCombate(p, &e);

        if (p->vida > 0) {
            aumentoStats(p, p->pos + 1);
        }

        p->pos++;

        if (p->pos == 3) {
            enviarTexto("El siguiente enemigo sera el jefe final de esta aventura\n");
            sleep(3);
        }

        if (p->pos == 4) {
            enviarTexto("Enhorabuena has terminado tu aventura\n");
            sleep(3);
            eliminarPartida(p->idJugador);
            free(p);
            salir();
            break;
        }

        guardarPartida(p->idJugador, p->pos, p->vida, p->armadura, p->velocidad, p->veces, p->ataque);
        enviarTexto("\n+ Partida guardada +\n");
    }
    free(p);
}
