#ifndef COMBATE_H
#define COMBATE_H




#include "personaje.h" // Para usar la estructura del personaje

typedef struct {
    char nombre[20];
    int ataque;
    int armadura;
    int vida;
    int velocidad;
} Enemigo;

void iniciarCombate(Clase *jugador, Enemigo *enemigo);

#endif