#ifndef COMBATE_H
#define COMBATE_H

#include "personaje.h" // Para usar la estructura del personaje
#include <winsock2.h>

typedef struct {
    char nombre[30];
    int ataque;
    int veces;
    int armadura;
    int vida;
    int velocidad;
} Enemigo;

void iniciarCombate(Clase *jugador, Enemigo *enemigo);
void turnoPersonaje(Clase *jugador, Enemigo *enemigo, FILE *logFile);
void turnoEnemigo(Clase *jugador, Enemigo *enemigo, FILE *logFile);
void iniciarCombateOnline(Clase *jugador, Enemigo *enemigo, SOCKET sock);
void turnoPersonajeOnline(Clase *jugador, Enemigo *enemigo, FILE *logFile, SOCKET sock);
void turnoEnemigoOnline(Clase *jugador, Enemigo *enemigo, FILE *logFile, SOCKET sock);

#endif