#ifndef PROGRESION_H
#define PROGRESION_H

#include <winsock2.h>
#include "personaje.h"

void aumentoStats(Clase *p, int sala);
void aumentoStatsOnline(Clase *p, int sala, SOCKET sock);

#endif