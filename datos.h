// datos.h
#ifndef DATOS_H
#define DATOS_H

int inicializarBD();
void cerrarBD();
int guardarPartida(int idJugador, const char *datos);
int insertarClases();
int insertarEnemigos();

#endif // DATOS_H
