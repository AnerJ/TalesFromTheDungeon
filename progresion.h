#ifndef PROGRESION_H
#define PROGRESION_H
#include "personaje.h"


typedef struct {
    int idJugador;                        // ID del jugador en la BD
    char nombreJugador[50];               // Nombre del jugador
    Clase estadisticas;                        // Estadísticas actualizadas del jugador
    int salaActual;                       // Número de sala actual en la mazmorra
    char enemigosEliminados[256];         // Lista de enemigos derrotados (por ejemplo, "Esqueleto,Golem")
    char enemigosRestantes[256];          // Lista de enemigos que faltan ("Espectro,Nigromante")
} GameState;

void aumentoStats(Clase *p, int sala);

int cargarPartidaDB(int idJugador, char *buffer, int bufferSize);

#endif

