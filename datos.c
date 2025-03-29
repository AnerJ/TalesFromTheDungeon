// datos.c
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "datos.h"
#include "personaje.h"
#include "combate.h"

static sqlite3 *db = NULL;

sqlite3* inicializarBD() {
    int rc = sqlite3_open("talesfromthedungeon.db", &db);
    if(rc) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    char *errMsg = NULL;

    const char *sqlEstadisticas = "CREATE TABLE IF NOT EXISTS Estadisticas ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "vida INTEGER, armadura INTEGER, velocidad INTEGER, "
                                  "veces INTEGER, ataque INTEGER);";
    rc = sqlite3_exec(db, sqlEstadisticas, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Estadisticas: %s\n", errMsg);
        sqlite3_free(errMsg);
        return NULL;
    }

    const char *sqlClases = "CREATE TABLE IF NOT EXISTS Clases ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "nombre TEXT NOT NULL, idEstadistica INTEGER, "
                            "FOREIGN KEY (idEstadistica) REFERENCES Estadisticas(id));";
    rc = sqlite3_exec(db, sqlClases, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Clases: %s\n", errMsg);
        sqlite3_free(errMsg);
        return NULL;
    }

    const char *sqlJugador = "CREATE TABLE IF NOT EXISTS Jugador ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "nombre TEXT NOT NULL, idClase INTEGER, "
                             "FOREIGN KEY (idClase) REFERENCES Clases(id));";
    rc = sqlite3_exec(db, sqlJugador, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Jugador: %s\n", errMsg);
        sqlite3_free(errMsg);
        return NULL;
    }

    const char *sqlEnemigos = "CREATE TABLE IF NOT EXISTS Enemigos ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "nombre TEXT NOT NULL, idEstadistica INTEGER, "
                              "FOREIGN KEY (idEstadistica) REFERENCES Estadisticas(id));";
    rc = sqlite3_exec(db, sqlEnemigos, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Enemigos: %s\n", errMsg);
        sqlite3_free(errMsg);
        return NULL;
    }

    const char *sqlPartidas = "CREATE TABLE IF NOT EXISTS Partidas ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "nombre TEXT NOT NULL, idJugador INTEGER, "
                              "salaActual INTEGER, "
                              "fechaGuardado DATETIME DEFAULT CURRENT_TIMESTAMP, "
                              "FOREIGN KEY (idJugador) REFERENCES Jugador(id));";
    rc = sqlite3_exec(db, sqlPartidas, 0, 0, &errMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error creando tabla Partidas: %s\n", errMsg);
        sqlite3_free(errMsg);
        return NULL;
    }

    return db;
};

void cerrarBD() {
    if(db)
        sqlite3_close(db);
}


int insertarClases() {


    if (existeEnTabla("Clases")) {
        printf("Las clases ya han sido insertadas previamente.\n");
        return 0; // No insertamos de nuevo
    }

    const char *sqlEstadisticas = "INSERT INTO Estadisticas (vida, armadura, velocidad, veces, ataque) VALUES (?, ?, ?, ?, ?);";
    const char *sqlClases = "INSERT INTO Clases (nombre, idEstadistica) VALUES (?, last_insert_rowid());";

    sqlite3_stmt *stmtEstadisticas, *stmtClases;

    // Datos de las clases
    const char *clases[3] = {"Guerrero", "Cazador", "Pícaro"};
    int estadisticas[3][5] = {
        {20, 7, 2, 1, 12},  // Guerrero (1d12)
        {15, 5, 5, 2, 8},   // Cazador (2d8)
        {13, 2, 8, 3, 5}    // Pícaro (3d5)
    };

    int rc;
    for (int i = 0; i < 3; i++) {
        // Insertar en Estadisticas
        rc = sqlite3_prepare_v2(db, sqlEstadisticas, -1, &stmtEstadisticas, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando statement para Estadisticas: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        for (int j = 0; j < 5; j++) {
            sqlite3_bind_int(stmtEstadisticas, j + 1, estadisticas[i][j]);
        }

        rc = sqlite3_step(stmtEstadisticas);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error insertando en Estadisticas: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmtEstadisticas);
            return rc;
        }
        sqlite3_finalize(stmtEstadisticas);

        // Insertar en Clases (usando last_insert_rowid() para obtener el ID recién generado)
        rc = sqlite3_prepare_v2(db, sqlClases, -1, &stmtClases, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando statement para Clases: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        sqlite3_bind_text(stmtClases, 1, clases[i], -1, SQLITE_STATIC);

        rc = sqlite3_step(stmtClases);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error insertando en Clases: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmtClases);
            return rc;
        }

        sqlite3_finalize(stmtClases);
    }

    return 0; // Éxito
}

int insertarEnemigos() {
    if (existeEnTabla("Enemigos")) {
        printf("Los enemigos ya han sido insertados previamente.\n");
        return 0; // No insertamos de nuevo
    }

    const char *sqlEstadisticas = "INSERT INTO Estadisticas (vida, armadura, velocidad, veces, ataque) VALUES (?, ?, ?, ?, ?);";
    const char *sqlEnemigos = "INSERT INTO Enemigos (nombre, idEstadistica) VALUES (?, last_insert_rowid());";

    sqlite3_stmt *stmtEstadisticas, *stmtEnemigos;

    // Datos de los enemigos
    const char *enemigos[4] = {"Esqueleto", "Golem", "Espectro Deambulante", "Nigromante"};
    int estadisticas[4][5] = {
        {10, 5, 1, 1, 4},  // Esqueleto (1d4)
        {7, 1, 4, 3, 10},  // Golem (3d10)
        {13, 5, 2, 5, 4},  // Espectro Deambulante (5d4)
        {10, 7, 7, 2, 20}  // Nigromante (2d20)
    };

    int rc;
    for (int i = 0; i < 4; i++) {
        // Insertar en Estadisticas
        rc = sqlite3_prepare_v2(db, sqlEstadisticas, -1, &stmtEstadisticas, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando statement para Estadisticas: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        for (int j = 0; j < 5; j++) {
            sqlite3_bind_int(stmtEstadisticas, j + 1, estadisticas[i][j]);
        }

        rc = sqlite3_step(stmtEstadisticas);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error insertando en Estadisticas: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmtEstadisticas);
            return rc;
        }
        sqlite3_finalize(stmtEstadisticas);

        // Insertar en Enemigos (usando last_insert_rowid() para obtener el ID recién generado)
        rc = sqlite3_prepare_v2(db, sqlEnemigos, -1, &stmtEnemigos, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparando statement para Enemigos: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        sqlite3_bind_text(stmtEnemigos, 1, enemigos[i], -1, SQLITE_STATIC);

        rc = sqlite3_step(stmtEnemigos);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error insertando en Enemigos: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmtEnemigos);
            return rc;
        }

        sqlite3_finalize(stmtEnemigos);
    }

    return 0; // Éxito
}


int cargarClase(int idClase, Clase *clase) {
    const char *sql = "SELECT E.vida, E.armadura, E.velocidad, E.veces, E.ataque "
                      "FROM Clases AS C "
                      "INNER JOIN Estadisticas AS E ON C.idEstadistica = E.id "
                      "WHERE C.id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, idClase);

    rc = sqlite3_step(stmt);
    if(rc == SQLITE_ROW) {
        clase->vida = sqlite3_column_int(stmt, 0);
        clase->armadura = sqlite3_column_int(stmt, 1);
        clase->velocidad = sqlite3_column_int(stmt, 2);
        clase->veces = sqlite3_column_int(stmt, 3);
        clase->ataque = sqlite3_column_int(stmt, 4);
    } else {
        fprintf(stderr, "Clase no encontrada\n");
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int cargarEnemigos(Enemigo *enemigos, int cantidadEnemigos) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT E.nombre, ES.ataque, ES.veces, ES.vida, ES.armadura, ES.velocidad "
                      "FROM Enemigos AS E "
                      "INNER JOIN Estadisticas AS ES ON E.idEstadistica = ES.id "
                      "WHERE E.id BETWEEN 1 AND 4;";
    int rc;

    // Verificar que el array de enemigos no sea NULL
    if (enemigos == NULL) {
        fprintf(stderr, "Error: El array de enemigos es NULL.\n");
        return SQLITE_ERROR;
    }

    // Verificar que la cantidad de enemigos sea válida
    if (cantidadEnemigos <= 0) {
        fprintf(stderr, "Error: La cantidad de enemigos debe ser mayor que 0.\n");
        return SQLITE_ERROR;
    }

    // Preparar la consulta
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Ejecutar la consulta y cargar los datos de los enemigos
    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < cantidadEnemigos) {
        // Obtener los valores de cada columna
        const char *nombre = (const char*)sqlite3_column_text(stmt, 0);
        enemigos[i].ataque = sqlite3_column_int(stmt, 1);
        enemigos[i].veces = sqlite3_column_int(stmt, 2);
        enemigos[i].vida = sqlite3_column_int(stmt, 3);
        enemigos[i].armadura = sqlite3_column_int(stmt, 4);
        enemigos[i].velocidad = sqlite3_column_int(stmt, 5);

        // Copiar el nombre de manera segura
        snprintf(enemigos[i].nombre, sizeof(enemigos[i].nombre), "%s", nombre);

        i++;
    }

    // Finalizar el statement
    sqlite3_finalize(stmt);

    // Verificar si se cargaron todos los enemigos solicitados
    if (i < cantidadEnemigos) {
        fprintf(stderr, "Advertencia: Solo se encontraron %d enemigos (se solicitaron %d).\n", i, cantidadEnemigos);
    }

    return SQLITE_OK;
}

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

// Insertar nuevo jugador en la base de datos
int insertarJugador(const char *nombre, int idClase) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Jugador (nombre, idClase) VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando la inserción: %s\n", sqlite3_errmsg(db));
        return SQLITE_ERROR;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idClase);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error al insertar jugador: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return SQLITE_ERROR;
    }

    sqlite3_finalize(stmt);

    printf("✅ Jugador '%s' insertado correctamente con idClase %d.\n", nombre, idClase);
    return SQLITE_OK;
}