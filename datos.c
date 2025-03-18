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
    
    // Tabla Jugador
    const char *sqlJugador = "CREATE TABLE IF NOT EXISTS Jugador ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "nombre TEXT NOT NULL);";
    rc = sqlite3_exec(db, sqlJugador, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Jugador: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    // Tabla Clases
    const char *sqlClases = "CREATE TABLE IF NOT EXISTS Clases ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "nombre TEXT NOT NULL, "
                            "idJugador INTEGER, "
                            "FOREIGN KEY (idJugador) REFERENCES Jugador(id));";
    rc = sqlite3_exec(db, sqlClases, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Clases: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    // Tabla Estadisticas 
    const char *sqlEstadisticas = "CREATE TABLE IF NOT EXISTS Estadisticas ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "vida INTEGER, "
                                  "armadura INTEGER, "
                                  "velocidad INTEGER, "
                                  "ataque TEXT, "      // Ejemplo: "1d12"
                                  "idClase INTEGER, "
                                  "FOREIGN KEY (idClase) REFERENCES Clases(id));";
    rc = sqlite3_exec(db, sqlEstadisticas, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Estadisticas: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    // Tabla Enemigos
    const char *sqlEnemigos = "CREATE TABLE IF NOT EXISTS Enemigos ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "nombre TEXT NOT NULL, "
                              "vida INTEGER, "
                              "armadura INTEGER, "
                              "velocidad INTEGER, "
                              "precision INTEGER);";
    rc = sqlite3_exec(db, sqlEnemigos, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Enemigos: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    // Tabla de relación entre Estadisticas y Enemigos 
    const char *sqlEstadisticasEnemigos = "CREATE TABLE IF NOT EXISTS Estadisticas_Enemigos ("
                                          "idEstadistica INTEGER, "
                                          "idEnemigo INTEGER, "
                                          "PRIMARY KEY (idEstadistica, idEnemigo), "
                                          "FOREIGN KEY (idEstadistica) REFERENCES Estadisticas(id), "
                                          "FOREIGN KEY (idEnemigo) REFERENCES Enemigos(id));";
    rc = sqlite3_exec(db, sqlEstadisticasEnemigos, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Estadisticas_Enemigos: %s\n", errMsg);
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
    
    // Tabla de relación entre Estadisticas y Ataques
    const char *sqlEstadisticasAtaques = "CREATE TABLE IF NOT EXISTS Estadisticas_Ataques ("
                                         "idEstadistica INTEGER, "
                                         "idAtaque INTEGER, "
                                         "PRIMARY KEY (idEstadistica, idAtaque), "
                                         "FOREIGN KEY (idEstadistica) REFERENCES Estadisticas(id), "
                                         "FOREIGN KEY (idAtaque) REFERENCES Ataques(id));";
    rc = sqlite3_exec(db, sqlEstadisticasAtaques, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Estadisticas_Ataques: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    // Relación muchos a muchos entre Enemigos y Ataques
    const char *sqlEnemigosAtaques = "CREATE TABLE IF NOT EXISTS Enemigos_Ataques ("
                                     "idEnemigo INTEGER, "
                                     "idAtaque INTEGER, "
                                     "PRIMARY KEY (idEnemigo, idAtaque), "
                                     "FOREIGN KEY (idEnemigo) REFERENCES Enemigos(id), "
                                     "FOREIGN KEY (idAtaque) REFERENCES Ataques(id));";
    rc = sqlite3_exec(db, sqlEnemigosAtaques, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Enemigos_Ataques: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    
    // Tabla Partidas 
    const char *sqlPartidas = "CREATE TABLE IF NOT EXISTS Partidas ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "idJugador INTEGER, "
                              "datos TEXT, "
                              "FOREIGN KEY (idJugador) REFERENCES Jugador(id));"
                              "FOREIGN KEY (idEnemigo) REFERENCES Enemigos(id));"
                              "FOREIGN KEY (idEstadistica) REFERENCES Estadisticas(id));";

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
