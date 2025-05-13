#ifndef SALIRDELJUEGO_H
#define SALIRDELJUEGO_H
#include <winsock2.h>

void salir();
void menuP();
void mostrarMenu();
void nuevaPartida();
void cargarPartida();
void inciarPartida(Clase *p);
void inciarPartidaOnline(Clase *p, SOCKET sock);

#endif