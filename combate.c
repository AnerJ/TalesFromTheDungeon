#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "combate.h"
#include "personaje.h"

// Simulación de una tirada de dados
int lanzar_dado(int cantidad, int caras) {
    int resultado = 0;
    for (int i = 0; i < cantidad; i++) {
        resultado += (rand() % caras) + 1;
    }
    return resultado;
}

// Función de combate por turnos
void iniciarCombate(Clase *jugador, Enemigo *enemigo) {
    printf("\n¡Un %s ha aparecido!\n", enemigo->nombre);
    printf("El combate comienza...\n");

    // Bucle del combate
    while (jugador->vida > 0 && enemigo->vida > 0) {
        printf("\nTurno del jugador\n");
        printf("1. Atacar\n2. Defender\n3. Esquivar\n");
        int opcion;
        char c[10];
        fgets(c, 10, stdin);

        //Eliminar el salto de linea
        if( c[strlen(c)-1] == '\n'){
            c[strlen(c)-1] = '\0';
        }

        //Pasar el valor a entero
        sscanf(c, "%d", &opcion );
        

        if (opcion == 1) { // Ataque
            int danyo = lanzar_dado(1, 12) - enemigo->armadura;
            if (danyo < 0) danyo = 0;
            enemigo->vida -= danyo;
            printf("Has hecho %d de daño al %s!\n", danyo, enemigo->nombre);
        }

        if (enemigo->vida <= 0) {
            printf("¡Has derrotado al %s!\n", enemigo->nombre);
            break;
        }

        // Turno del enemigo
        printf("\nTurno del %s\n", enemigo->nombre);
        int danyo_enemigo = lanzar_dado(1, 10) - jugador->armadura;
        if (danyo_enemigo < 0) danyo_enemigo = 0;
        jugador->vida -= danyo_enemigo;
        printf("El %s te ha hecho %d de daño.\n", enemigo->nombre, danyo_enemigo);

        if (jugador->vida <= 0) {
            printf("¡Has sido derrotado!\n");
            break;
        }
    }
}
