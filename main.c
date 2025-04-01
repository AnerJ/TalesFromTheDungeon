#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include "personaje.h"
#include "menus.h"
#include "combate.h"
#include "datos.h"
#include "mazmorra.h"
#include "progresion.h"

void mostrarMenu();
void nuevaPartida();
void cargarPartida();
void inciarPartida(Clase *p);

int main(){
    // Configurar la consola en UTF-8 para poder mostrar la 'ñ', la '¡' y las tildes
    SetConsoleOutputCP(CP_UTF8);
    
    //Inicializacion de la base de datos
    inicializarBD();
    insertarClases();
    insertarEnemigos();
    
 
    menuP();

    return 0;
}

