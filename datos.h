
#ifndef DATOS_H
#define DATOS_H

#include "sqlite3.h"
#include "personaje.h"
#include "combate.h"

sqlite3* inicializarBD();
void cerrarBD();

int insertarClases();
int insertarEnemigos();

int insertarJugador(const char *nombre, int idClase);
int cargarClase(int idClase, Clase *clase);
int cargarEnemigos(Enemigo *enemigos, int cantidadEnemigos);
int existeEnTabla(const char *nombreTabla);


#endif
