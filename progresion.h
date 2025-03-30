#ifndef PROGRESION_H
#define PROGRESION_H
#include "personaje.h"
#include "sqlite3.h"


void aumentoStats(Clase *p, int sala);
int guardarPartida(sqlite3 *db, const char *nombrePartida, int idJugador, int salaActual);
int cargarPartida(sqlite3 *db, int *idJugador, int *salaActual);


#endif

