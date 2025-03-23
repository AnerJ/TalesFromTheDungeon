#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include "personaje.h"
#include "salirDelJuego.h"
#include "combate.h"
#include "datos.h"
#include "mazmorra.h"
#include "progresion.h"

void mostrarMenu();
void nuevaPartida();
void cargarPartida();
void inciarPartida(Clase *p);

int main(){
    // Configurar la consola en UTF-8 para poder mostrar la 'ñ', la '¡' y las tildes
    SetConsoleOutputCP(CP_UTF8);  // Configurar la consola en UTF-8
    //Inicializacion de la base de datos
    inicializarBD();
    insertarClases();
    insertarEnemigos();

    char promt[10];
    int opcion;
    do {
        mostrarMenu();
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

    return 0;
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
    //Llamara al modulo para cargar una partida
}

void inciarPartida(Clase *p){
    //Inicializacion de los enemigos
    int cantidadDeEnemigos = 4;
    Enemigo enemigos[4];
    cargarEnemigos(enemigos, cantidadDeEnemigos);

    int pos = 0;
    char txt[10];
    int accion;
    
    printf("Te adentras a la mazmorra...\n");
    sleep(2);
     
    while (p->vida >= 0){

        mostrarMapa("ficheros/mazmorraMapa.txt", pos + 1);

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
        accionesM(accion);
        Enemigo e = enemigos[pos];
        iniciarCombate( p, &e);
        
        if (p->vida > 0){
            aumentoStats(p, pos+ 1);
        }
        
        if (pos + 1 == 4){
            printf("El siguiente enemigo sera el jefe final de esta aventura\n");
            sleep(1);
        }

        pos ++;

        if (pos == 4){
            printf("Enhorabuena has terminado tu aventura");
            free(p);
            salir();
            break;
        }
    }
    free(p);
}