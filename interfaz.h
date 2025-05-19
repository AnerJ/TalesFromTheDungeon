#ifndef INTERFAZ_H
#define INTERFAZ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <winsock2.h>

void setSocketCliente(SOCKET s);
void getInput(char* buffer, int len);
void enviarTexto(const char* texto);

#ifdef __cplusplus
}
#endif

#endif
