#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void mostrarMenu();
void nuevaPartida();
void cargarPartida();


int main(){
    char promt[10];
    int opcion;

    do {
        mostrarMenu();
        printf("Selecciona una opcion introduciendo el numero: \n");
        fgets(promt, 10, stdin);

        //Eliminar el salto de linea
        if( promt[strlen(promt)-1] == '\n'){
            promt[strlen(promt)-1] = '\0';
        }

        //Pasar el valor a entero
        sscanf(promt, "%d", &opcion );


        if (opcion == 1){
            nuevaPartida();
            

        } else if (opcion == 2){
            cargarPartida();
            
        } else if (opcion == 3){
            printf("Saliendo... \n");
        }
        

        

    } while (opcion != 3);





    return 0;
}

void mostrarMenu(){
    printf("\n=================================\n");
    printf("TALES FROM THE DUNGEON\n");
    printf("=================================\n");
    printf("1. Nueva Partida\n");
    printf("2. Cargar Partida\n");
    printf("3. Salir\n");
}


void nuevaPartida(){
    printf("Creando nueva partida... \n");
    //Llamara al modulo para crear una nueva partida

}

void cargarPartida(){
    printf("Cargando partida... \n");
    //Llamara al modulo para cargar una partida
}


