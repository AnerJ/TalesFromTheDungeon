#include <stdio.h>
#include <stdlib.h>
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
void iniciarCombate(Personaje *jugador, Enemigo *enemigo) {
    printf("\n¡Un %s ha aparecido!\n", enemigo->nombre);
    printf("El combate comienza...\n");

    // Bucle del combate
    while (jugador->vida > 0 && enemigo->vida > 0) {
        printf("\nTurno del jugador\n");
        printf("1. Atacar\n2. Defender\n3. Esquivar\n");
        int opcion;
        scanf("%d", &opcion);

        if (opcion == 1) { // Ataque
            int daño = lanzar_dado(1, 12) - enemigo->armadura;
            if (daño < 0) daño = 0;
            enemigo->vida -= daño;
            printf("Has hecho %d de daño al %s!\n", daño, enemigo->nombre);
        }

        if (enemigo->vida <= 0) {
            printf("¡Has derrotado al %s!\n", enemigo->nombre);
            break;
        }

        // Turno del enemigo
        printf("\nTurno del %s\n", enemigo->nombre);
        int daño_enemigo = lanzar_dado(1, 10) - jugador->armadura;
        if (daño_enemigo < 0) daño_enemigo = 0;
        jugador->vida -= daño_enemigo;
        printf("El %s te ha hecho %d de daño.\n", enemigo->nombre, daño_enemigo);

        if (jugador->vida <= 0) {
            printf("¡Has sido derrotado!\n");
            break;
        }
    }
}
