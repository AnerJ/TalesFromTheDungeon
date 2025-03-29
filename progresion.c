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

// Guarda el progreso del jugador
int guardarPartida(sqlite3 *db, const char *nombrePartida, int idJugador, int salaActual) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Partidas (nombre, idJugador, salaActual) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return SQLITE_ERROR;
    }

    sqlite3_bind_text(stmt, 1, nombrePartida, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idJugador);
    sqlite3_bind_int(stmt, 3, salaActual);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al guardar partida: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return SQLITE_ERROR;
    }

    sqlite3_finalize(stmt);
    printf("✅ Partida guardada correctamente.\n");
    return SQLITE_OK;
}


// Carga una partida guardada por el jugador
int cargarPartida(sqlite3 *db, int *idJugador, int *salaActual) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre, idJugador, salaActual, fechaGuardado FROM Partidas;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Error al leer partidas guardadas: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("📂 Partidas guardadas:\n");
    int count = 0;
    int ids[100];
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *nombre = sqlite3_column_text(stmt, 1);
        const unsigned char *fecha = sqlite3_column_text(stmt, 4);

        printf("%d. %s (Guardado: %s)\n", ++count, nombre, fecha);
        ids[count - 1] = id;
    }
    sqlite3_finalize(stmt);

    if (count == 0) {
        printf("❌ No hay partidas guardadas.\n");
        return 0;
    }

    printf("Selecciona una partida (1-%d): ", count);
    int eleccion;
    scanf("%d", &eleccion);

    if (eleccion < 1 || eleccion > count) {
        printf("❌ Elección inválida.\n");
        return 0;
    }

    char query[128];
    snprintf(query, sizeof(query), "SELECT idJugador, salaActual FROM Partidas WHERE id = %d;", ids[eleccion - 1]);
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, 0) != SQLITE_OK) {
        printf("Error al obtener partida: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *idJugador = sqlite3_column_int(stmt, 0);
        *salaActual = sqlite3_column_int(stmt, 1);
        printf("🔄 Partida cargada. Jugador %d empezará en la sala %d.\n", *idJugador, *salaActual);
    }

    sqlite3_finalize(stmt);
    return 1;
}
