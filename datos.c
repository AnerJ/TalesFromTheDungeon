// datos.c
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "datos.h"

static sqlite3 *db = NULL;

int inicializarBD() {
    int rc = sqlite3_open("talesfromthedungeon.db", &db);
    if(rc) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    char *errMsg = NULL;

    // Tabla Estadísticas (Cada estadística se conecta con Ataques)
    const char *sqlEstadisticas = "CREATE TABLE IF NOT EXISTS Estadisticas ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "vida INTEGER, "
                                  "armadura INTEGER, "
                                  "velocidad INTEGER, "
                                  "idAtaque INTEGER, "
                                  "FOREIGN KEY (idAtaque) REFERENCES Ataques(id));";
    rc = sqlite3_exec(db, sqlEstadisticas, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Estadisticas: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }

    // Tabla Ataques
    const char *sqlAtaques = "CREATE TABLE IF NOT EXISTS Ataques ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "veces INTEGER, "
                             "danio INTEGER);";
    rc = sqlite3_exec(db, sqlAtaques, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Ataques: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }

    // Tabla Clases (Cada clase se asocia a una Estadística)
    const char *sqlClases = "CREATE TABLE IF NOT EXISTS Clases ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "nombre TEXT NOT NULL, "
                            "idEstadistica INTEGER, "
                            "FOREIGN KEY (idEstadistica) REFERENCES Estadisticas(id));";
    rc = sqlite3_exec(db, sqlClases, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Clases: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }

    // Tabla Jugador (Ahora cada jugador está asociado a una Clase)
    const char *sqlJugador = "CREATE TABLE IF NOT EXISTS Jugador ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "nombre TEXT NOT NULL, "
                             "idClase INTEGER, "
                             "FOREIGN KEY (idClase) REFERENCES Clases(id));";
    rc = sqlite3_exec(db, sqlJugador, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Jugador: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }

    // Tabla Enemigos (Cada enemigo ahora tiene su propia estadística)
    const char *sqlEnemigos = "CREATE TABLE IF NOT EXISTS Enemigos ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "nombre TEXT NOT NULL, "
                              "idEstadistica INTEGER, "
                              "FOREIGN KEY (idEstadistica) REFERENCES Estadisticas(id));";
    rc = sqlite3_exec(db, sqlEnemigos, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Enemigos: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }

    // Tabla Partidas
    const char *sqlPartidas = "CREATE TABLE IF NOT EXISTS Partidas ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "idJugador INTEGER, "
                              "datos TEXT, "
                              "FOREIGN KEY (idJugador) REFERENCES Jugador(id));";
    rc = sqlite3_exec(db, sqlPartidas, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Partidas: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    return 0;
}

void cerrarBD() {
    if(db)
        sqlite3_close(db);
}

int guardarPartida(int idJugador, const char *datos) {
    const char *sql = "INSERT INTO Partidas (idJugador, datos) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    
    sqlite3_bind_int(stmt, 1, idJugador);
    sqlite3_bind_text(stmt, 2, datos, -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE) {
        fprintf(stderr, "Error ejecutando statement: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE ? 0 : rc;
}
