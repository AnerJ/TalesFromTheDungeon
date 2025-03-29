#ifndef PERSONAJE_H
#define PERSONAJE_H


typedef struct 
{
    int idJugador; 
    int ataque;
    int veces;
    int vida;
    int armadura;
    int velocidad;
    int claseID;
} Clase;

Clase* crearPersonaje();

#endif
