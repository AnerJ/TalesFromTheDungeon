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

void salir(){
    
    printf("\n=================================\n");
    printf("TALES FROM THE DUNGEON\n");
    printf("=================================\n\n");
    printf("\n\nSaliendo....\n");
    sleep(5);
    cerrarBD();
    exit(0);
}

void menuP(){
    
    char promt[10];
    int opcion;
    do {
        mostrarMenu(); //Esto es lo que se sustituye
        printf("Selecciona una opcion introduciendo el numero: \n");
        fgets(promt, 10, stdin);

        //Eliminar el salto de linea
        if( promt[strlen(promt)-1] == '\n'){
            promt[strlen(promt)-1] = '\0';
        }

        //Pasar el valor a entero
        sscanf(promt, "%d", &opcion );

        if (opcion == 1){
            system("cls");
            nuevaPartida();
            break;
            
        } else if (opcion == 2){
            cargarPartida();
            break;
            
        } else if (opcion == 3){
            salir();
        }

    } while (opcion != 3);
}

void mostrarMenu(){
    printf("\n=================================\n");
    printf("TALES FROM THE DUNGEON\n");
    printf("=================================\n\n");
    sleep(3);
    printf("1. Nueva Partida\n");
    printf("2. Cargar Partida\n");
    printf("3. Salir\n\n");
}

void nuevaPartida(){
    printf("\n=================================\n");
    printf("TALES FROM THE DUNGEON\n");
    printf("=================================\n\n");
    printf("Creando nueva partida... \n");
    sleep(1);
    //Llamara al modulo para crear una nuevo personaje
    
    Clase *clase = crearPersonaje();
    if (clase == NULL){
        printf("Error al crear personaje\n");
    }
    inciarPartida(clase);
}

void cargarPartida(){
    printf("Cargando partida... \n");
    sleep(4);
    //Llamara al modulo para cargar una partida
    Clase *claseC = (Clase *)malloc(sizeof(Clase));
    mostrarPartidasGuardadas();
    char nombreP[10];
    int eleccion;

    printf("Elige la partida que quieres cargar introduciendo el numero: ");

    fgets(nombreP, 10, stdin);
    if( nombreP[strlen(nombreP)-1] == '\n'){
        nombreP[strlen(nombreP)-1] = '\0';
    }

    sscanf(nombreP, "%d", &eleccion );

    partidasCargadas(eleccion,claseC);
    printf("Datos Cargados");
    inciarPartida(claseC);
}

void inciarPartida(Clase *p){
    //Inicializacion de los enemigos
    int cantidadDeEnemigos = 4;
    Enemigo enemigos[4];
    cargarEnemigos(enemigos, cantidadDeEnemigos);
    char txt[10];
    int accion;
    
    printf("\nTe adentras a la mazmorra...\n");
    sleep(2);
     
    while (p->vida > 0){
        mostrarMapa("ficheros/mazmorraMapa.txt", p->pos + 1);

        printf("Cual es tu siguiente accion: \n"
                "1. Avanzar\n"
                "2. Huir\n");
        fgets(txt, 10, stdin);

        //Eliminar el salto de linea
        if( txt[strlen(txt)-1] == '\n'){
            txt[strlen(txt)-1] = '\0';
        }

        //Pasar el valor a entero
        sscanf(txt, "%d", &accion );

        if (accion == 5){
            salir();
        }

        accionesM(accion);
        Enemigo e = enemigos[p->pos];
        iniciarCombate( p, &e);

        
        
        if (p->vida > 0){
            aumentoStats(p, p->pos+ 1);
        }

        p->pos ++;

        if (p->pos  == 3){
            printf("El siguiente enemigo sera el jefe final de esta aventura\n");
            sleep(3);
        }

        
        
        if (p->pos == 4){
            printf("Enhorabuena has terminado tu aventura\n");
            sleep(3);
            eliminarPartida(p->idJugador); //Elimina la partida una vez se acaba la mazmorra


            free(p);
            salir();
            break;
        }

        guardarPartida(p->idJugador, p->pos, p->vida, p->armadura, p->velocidad, p->veces, p->ataque);
        printf("\n+ Partida guardada +\n");
    }
    free(p);
}