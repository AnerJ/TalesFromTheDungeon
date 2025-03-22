#ifndef PERSONAJE_H
#define PERSONAJE_H


typedef struct 
{
    int ataque;
    int veces;
    int vida;
    int armadura;
    int velocidad;
} Clase;

Clase* crearPersonaje();

#endif
