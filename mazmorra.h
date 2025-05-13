#ifndef MAZMORRA_H
#define MAZMORRA_H
#include <winsock2.h>

void mostrarMapa( const char * rutaFichero,int pos);
void accionesM(int accion);
void mostrarMapaOnline(const char *rutaFichero, int pos, SOCKET sock);

#endif