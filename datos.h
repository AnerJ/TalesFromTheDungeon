// datos.h
#ifndef DATOS_H
#define DATOS_H

#include "personaje.h"
int inicializarBD();
void cerrarBD();
int guardarPartida(int idJugador, const char *datos);
int insertarClases();
int insertarEnemigos();
int cargarClase(int idClase, Clase *clase);
int cargarEnemigos(Enemigo *enemigos, int cantidadEnemigos);

#endif // DATOS_H
