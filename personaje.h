#ifndef PERSONAJE_H
#define PERSONAJE_H


typedef struct 
{
    char nombre[50];
    int idJugador;
    int ataque;
    int veces;
    int vida;
    int armadura;
    int velocidad;
    int pos;
} Clase;

Clase* crearPersonaje();

#endif
