#ifndef COMBATE_H
#define COMBATE_H




#include "personaje.h" // Para usar la estructura del personaje

typedef struct {
    char nombre[25];
    int ataque;
    int veces;
    int armadura;
    int vida;
    int velocidad;
} Enemigo;

void iniciarCombate(Clase *jugador, Enemigo *enemigo);
void turnoPersonaje(Clase *jugador, Enemigo *enemigo, FILE *logFile);
void turnoEnemigo(Clase *jugador, Enemigo *enemigo, FILE *logFile);

#endif