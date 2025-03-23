#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "combate.h"
#include "personaje.h"
#include "salirDelJuego.h"
#include <unistd.h>

// Simulación de una tirada de dados
int lanzar_dado(int veces, int danyo) {
    int resultado = 0;
    for (int i = 0; i < veces; i++) {
        resultado += (rand() % danyo) + 1;
    }
    return resultado;
}

// Función de combate por turnos
void iniciarCombate(Clase *jugador, Enemigo *enemigo) {
    FILE *logFile = fopen("combate_log.txt", "a");
    if (logFile == NULL) {
        printf("Error al abrir el archivo de log.\n");
        return;
    }
    fprintf(logFile, "\n---------------------------------------------\n");
    fprintf(logFile, "\n++++++++++++++++++++++++++++++\n");
    fprintf(logFile, "+  Enemigo: %s\n", enemigo->nombre);
    fprintf(logFile, "++++++++++++++++++++++++++++++\n");

    printf("\n¡Un %s ha aparecido!\n", enemigo->nombre);
    printf("El combate comienza...\n");

    // Bucle del combate
    while (jugador->vida > 0 && enemigo->vida > 0) {
        sleep(1);
        printf("\n*****************\n");
        fprintf(logFile, "\n*****************\n");

        //Enseñamos las estadisticas
        printf("Tus estadisticas                Estadisticas del Enemigo\n "
            "Vida: %d                       Vida: %d\n"
            "Ataque: %dd%d                  Ataque: %dd%d\n"
            "Armadura: %d                   Armadura: %d\n"
            "Velocidad: %d                  Velocidad: %d",
            jugador->vida, enemigo->vida, jugador->veces, jugador->ataque, enemigo->veces, enemigo->ataque, jugador->armadura, enemigo->armadura,
            jugador->velocidad, enemigo->velocidad
        );
        fprintf(logFile, "Tu                            Enemigo\n "
            "Vida: %d                       Vida: %d\n"
            "Ataque: %dd%d                  Ataque: %dd%d\n"
            "Armadura: %d                   Armadura: %d\n"
            "Velocidad: %d                  Velocidad: %d",
            jugador->vida, enemigo->vida, jugador->veces, jugador->ataque, enemigo->veces, enemigo->ataque, jugador->armadura, enemigo->armadura,
            jugador->velocidad, enemigo->velocidad
        );

        printf("\n*****************\n");
        fprintf(logFile, "\n*****************\n");
        sleep(1);

        // Inicio del turno del jugador
        printf("\nTurno del jugador\n");
        fprintf(logFile, "\nTurno del jugador\n");
        sleep(2);
        printf("1. Atacar\n2. Defender\n");
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
            int danyo = lanzar_dado(jugador->veces, jugador->ataque) - enemigo->armadura;
            if (danyo < 0) danyo = 0;
            enemigo->vida -= danyo;
            system("cls");
            printf("Has hecho %d de daño al %s!\n", danyo, enemigo->nombre);
            fprintf(logFile, "Has hecho %d de daño al %s!\n", danyo, enemigo->nombre);
            sleep(1);
        }
        else if (opcion == 2){ //Defensa
            jugador->armadura += 5;
            system("cls");
            printf("Vas a bloquear el siguiente ataque con %d de armadura", jugador->armadura);
            fprintf(logFile, "Bloqueo con %d de armadura", jugador->armadura);
            sleep(1);
        }

        if (enemigo->vida <= 0) {
            printf("¡Has derrotado al %s!\n", enemigo->nombre);
            fprintf(logFile, "\n++++++++++++++++++++++++++++++\n");
            fprintf(logFile, "*  %s derrotado\n", enemigo->nombre);
            fprintf(logFile, "++++++++++++++++++++++++++++++\n");
            fprintf(logFile, "\n---------------------------------------------\n");
            break;
        }

        // Inicio del turno del enemigo
        printf("\nTurno del %s\n", enemigo->nombre);
        fprintf(logFile, "\nTurno del %s\n", enemigo->nombre);
        sleep(1);
        int danyo_enemigo = lanzar_dado(enemigo->veces, enemigo->ataque) - jugador->armadura;
        if (danyo_enemigo < 0) danyo_enemigo = 0;
        jugador->vida -= danyo_enemigo;
        printf("El %s te ha hecho %d de daño.\n", enemigo->nombre, danyo_enemigo);
        fprintf(logFile, "El %s te ha hecho %d de daño.\n", enemigo->nombre, danyo_enemigo);
        sleep(1);
        if (opcion == 2){
            jugador->armadura -= 5;
        }

        if (jugador->vida <= 0) {
            printf("Has sido derrotado!\n");
            fprintf(logFile, "GAME OVER!\n");
            break;
        }

        // Fin del turno del enemigo
        fprintf(logFile, "\n---------------------------------------------\n");
    }

    fclose(logFile);
}
