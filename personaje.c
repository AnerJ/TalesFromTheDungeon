#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "personaje.h"
#include "salirDelJuego.h"
#include "datos.h"



Clase * crearPersonaje(){
    Clase * personaje = NULL;
    personaje = (Clase*) malloc (sizeof(Clase));
    printf("\n===============\n"
            "Creacion de Personaje\n"
            "===============\n");
    sleep(2);
    printf("\nDime tu nombre valiente aventurero: \n");
    


    char nombre[10];
    //char* nombre1;
    
    fgets(nombre, sizeof(nombre), stdin);
    

        //Eliminar el salto de linea
    if( nombre[strlen(nombre)-1] == '\n'){
            nombre[strlen(nombre)-1] = '\0';
    }

    //sscanf(nombre, "%c", &nombre1 );

    
    printf("Valiente aventurero %s, Elige tu clase: \n", nombre);
    strcpy(personaje->nombre, nombre);

    printf("\n*****************\n"
        "| 1. Guerrero   |\n"
        "| Ataque: 1d12  |\n"
        "| Armadura: 7   |\n"
        "| Vida: 20      |\n"
        "| Velocidad: 2  |\n"
        "*****************\n");
    sleep(2);
         
    printf("\n*****************\n"
        "| 2. Cazador    |\n"
        "| Ataque: 2d8   |\n"
        "| Armadura: 5   |\n"
        "| Vida: 15      |\n"
        "| Velocidad: 5  |\n"
        "*****************\n");
    sleep(2);
    
    printf("\n*****************\n"
        "| 3. Picaro     |\n"
        "| Ataque: 3d5   |\n"
        "| Armadura: 2   |\n"
        "| Vida: 13      |\n"
        "| Velocidad: 8  |\n"
        "*****************\n");
    sleep(2); 

    printf("\nElige una clase: \n");


    char promt2[10];
    int opcion2;
    
    fgets(promt2, 10, stdin);

        //Eliminar el salto de linea
    if( promt2[strlen(promt2)-1] == '\n'){
            promt2[strlen(promt2)-1] = '\0';
    }

    


    //Pasar el valor a entero
    sscanf(promt2, "%d", &opcion2 );

        if (opcion2 == 1)
        {
            system("cls");
            printf("\nHas elegido el Guerrero\n\n");
            cargarClase(opcion2, personaje);
            
        } else if (opcion2 == 2){
            printf("\nHas elegido el Cazador\n");
            cargarClase(opcion2, personaje);

        } else if (opcion2 == 3){
            printf("\nHas elegido el Picaro");
            cargarClase(opcion2, personaje);

        } else if (opcion2 == 5){
            salir();
        }
        personaje->pos = 0;
        personaje->idJugador =insertarJugador(nombre, opcion2);

        return personaje;
}