// datos.h
#ifndef DATOS_H
#define DATOS_H

#include "personaje.h"
#include "combate.h"
int inicializarBD();
void cerrarBD();
int guardarPartida(int idJugador, int posicionSala, int vida, int armadura, int velocidad, int veces, int ataque);
int insertarClases();
int insertarEnemigos();
int cargarClase(int idClase, Clase *clase);
int cargarEnemigos(Enemigo *enemigos, int cantidadEnemigos);
int insertarJugador(const char *nombre, int idClase);
void partidasCargadas(int id, Clase *clase);
void mostrarPartidasGuardadas();



#endif // DATOS_H
