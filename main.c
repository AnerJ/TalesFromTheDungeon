
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
void cargarPartidaMenu();
void iniciarPartida(Clase *p, int salaActual);

int main(){
    SetConsoleOutputCP(CP_UTF8);
    inicializarBD();
    insertarClases();
    insertarEnemigos();

    char promt[10];
    int opcion;
    do {
        mostrarMenu();
        printf("Selecciona una opcion introduciendo el numero: \n");
        fgets(promt, 10, stdin);

        if( promt[strlen(promt)-1] == '\n'){
            promt[strlen(promt)-1] = '\0';
        }

        sscanf(promt, "%d", &opcion );

        if (opcion == 1){
            system("cls");
            nuevaPartida();
            break;

        } else if (opcion == 2){
            cargarPartidaMenu();
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

    Clase *clase = crearPersonaje();
    if (clase == NULL){
        printf("Error al crear personaje\n");
    }
    int salaActual = 0;
    iniciarPartida(clase, salaActual);
}

void cargarPartidaMenu(){
    int idJugador, salaActual;
    sqlite3 *db = inicializarBD();
    if (db == NULL) {
        fprintf(stderr, "Error al inicializar la base de datos.\n");
        return;
    }
    
    if (!cargarPartida(db, &idJugador, &salaActual)) {
        sqlite3_close(db);
        return;
    }
    
    Clase *clase = malloc(sizeof(Clase));
    if (cargarClase(idJugador, clase) != SQLITE_OK) {
        printf("Error al cargar la clase\n");
        free(clase);
        sqlite3_close(db);
        return;
    }

    clase->idJugador = idJugador;
    sqlite3_close(db);
    iniciarPartida(clase, salaActual);
}

void iniciarPartida(Clase *p, int salaActual){
    int cantidadDeEnemigos = 4;
    Enemigo enemigos[4];
    cargarEnemigos(enemigos, cantidadDeEnemigos);

    int pos = salaActual;
    char txt[10];
    int accion;

    printf("\nTe adentras a la mazmorra...\n");
    sleep(2);

    while (p->vida >= 0){
        mostrarMapa("ficheros/mazmorraMapa.txt", pos + 1);

        printf("Cual es tu siguiente accion: \n"
                "1. Avanzar\n"
                "2. Huir\n");
        fgets(txt, 10, stdin);

        if( txt[strlen(txt)-1] == '\n'){
            txt[strlen(txt)-1] = '\0';
        }

        sscanf(txt, "%d", &accion );

        if (accion == 5){
            salir();
        }

        accionesM(accion);
        Enemigo e = enemigos[pos];
        iniciarCombate(p, &e);

        if (p->vida > 0){
            aumentoStats(p, pos + 1);
        }

        // Guardar después del combate
        char nombrePartida[50];
        printf("Introduce un nombre para guardar la partida (o presiona ENTER para omitir): ");
        fgets(nombrePartida, sizeof(nombrePartida), stdin);

        if (nombrePartida[strlen(nombrePartida) - 1] == '\n') {
            nombrePartida[strlen(nombrePartida) - 1] = '\0';
        }

        if (strlen(nombrePartida) > 0) {
            sqlite3 *db = inicializarBD();
            if (!guardarPartida(db, nombrePartida, p->idJugador, pos)) {
                printf("❌ Error al guardar la partida.\n");
            } else {
                printf("💾 Partida guardada con éxito.\n");
            }
            sqlite3_close(db);
        }

        if (pos + 1 == 4){
            printf("El siguiente enemigo sera el jefe final de esta aventura\n");
            sleep(3);
        }

        pos++;

        if (pos == 4){
            printf("Enhorabuena has terminado tu aventura");
            sleep(3);
            free(p);
            salir();
            break;
        }
    }
    free(p);
}
