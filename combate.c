#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "combate.h"
#include "personaje.h"
#include "menus.h"
#include "interfaz.h"


int lanzar_dado(int veces, int danyo) {
    int resultado = 0;
    for (int i = 0; i < veces; i++) {
        resultado += (rand() % danyo) + 1;
    }
    return resultado;
}


void iniciarCombate(Clase *jugador, Enemigo *enemigo) {
    FILE *logFile = fopen("log/combate_log.txt", "a");
    if (logFile == NULL) {
        enviarTexto("Error al abrir el archivo de log.\n");
        return;
    }
    





    char buffer[512];
    snprintf(buffer, sizeof(buffer), "\n++++++++++++++++++++++++++++++\n+  Enemigo: %s\n++++++++++++++++++++++++++++++\n\n---------------------------------------------\n", enemigo->nombre);
    enviarTexto(buffer);
    fprintf(logFile, "%s", buffer);

    snprintf(buffer, sizeof(buffer), "\n\n\u00a1Un %s ha aparecido!\nEl combate comienza...\n", enemigo->nombre);
    enviarTexto(buffer);

    while (jugador->vida > 0 && enemigo->vida > 0) {
        sleep(1);
        enviarTexto("\n*****************\n");
        fprintf(logFile, "\n*****************\n");

        snprintf(buffer, sizeof(buffer),
            " Tus estadisticas               Estadisticas del Enemigo\n "
            "Vida: %d                       Vida: %d\n"
            " Ataque: %dd%d                   Ataque: %dd%d\n"
            " Armadura: %d                    Armadura: %d\n"
            " Velocidad: %d                   Velocidad: %d\n",
            jugador->vida, enemigo->vida,
            jugador->veces, jugador->ataque,
            enemigo->veces, enemigo->ataque,
            jugador->armadura, enemigo->armadura,
            jugador->velocidad, enemigo->velocidad);

        enviarTexto(buffer);
        fprintf(logFile, "%s", buffer);

        enviarTexto("*****************\n");
        fprintf(logFile, "\n*****************\n");
        sleep(1);
        int armaduraInicial = jugador->armadura;

        if (jugador->velocidad >= enemigo->velocidad) {
            enviarTexto("Eres mas rapido que tu rival\nAtacas primero\n");
            fprintf(logFile, "El jugador es mas rapido que el enemigo, ataca primero\n");
            sleep(1);
            turnoPersonaje(jugador, enemigo, logFile);
            if (enemigo->vida <= 0) {
                snprintf(buffer, sizeof(buffer), "\u00a1Has derrotado al %s!\n", enemigo->nombre);
                enviarTexto(buffer);
                fprintf(logFile, "\n++++++++++++++++++++++++++++++\n*  %s derrotado\n++++++++++++++++++++++++++++++\n\n---------------------------------------------\n", enemigo->nombre);
                break;
            }
            turnoEnemigo(jugador, enemigo, logFile);

            if (jugador->vida <= 0) {
                enviarTexto("Has sido derrotado!\n");
                fprintf(logFile, "GAME OVER!\n");
                salir();
                break;
            }
        } else {
            enviarTexto("El enemigo es mas rapido que tu\nAtacara primero\n");
            fprintf(logFile, "El enemigo es mas rapido que el jugador, el enemigo ataca primero\n");
            sleep(1);
            turnoEnemigo(jugador, enemigo, logFile);
            if (jugador->vida <= 0) {
                enviarTexto("Has sido derrotado!\n");
                fprintf(logFile, "GAME OVER!\n");
                salir();
                break;
            }
            turnoPersonaje(jugador, enemigo, logFile);
            if (enemigo->vida <= 0) {
                snprintf(buffer, sizeof(buffer), "\u00a1Has derrotado al %s!\n", enemigo->nombre);
                enviarTexto(buffer);
                fprintf(logFile, "\n++++++++++++++++++++++++++++++\n*  %s derrotado\n++++++++++++++++++++++++++++++\n\n---------------------------------------------\n", enemigo->nombre);
                break;
            }

        }


        if (jugador->armadura != armaduraInicial) {
            jugador->armadura = armaduraInicial;
        }


        fprintf(logFile, "\n---------------------------------------------\n");
    }

    fclose(logFile);
}

void turnoPersonaje(Clase *jugador, Enemigo *enemigo, FILE *logFile) {
    enviarTexto("\nTurno del jugador\n");
    fprintf(logFile, "\nTurno del jugador\n");
    sleep(1);

    enviarTexto("1. Atacar\n2. Defender\n");
    int opcion;
    char c[10];
    getInput(c, 10);
    if (c[strlen(c) - 1] == '\n') c[strlen(c) - 1] = '\0';

    sscanf(c, "%d", &opcion);

    char buffer[256];
    if (opcion == 1) {
        int danyo = lanzar_dado(jugador->veces, jugador->ataque) - enemigo->armadura;
        if (danyo < 0) danyo = 0;
        enemigo->vida -= danyo;
        snprintf(buffer, sizeof(buffer), "Has hecho %d de dan\u00f5 al %s!\n", danyo, enemigo->nombre);
        enviarTexto(buffer);
        fprintf(logFile, "%s", buffer);
    } else if (opcion == 2) {
        jugador->armadura += 5;
        snprintf(buffer, sizeof(buffer), "Vas a bloquear el siguiente ataque con %d de armadura\n", jugador->armadura);
        enviarTexto(buffer);
        fprintf(logFile, "%s", buffer);
    } else if (opcion == 4) {
        menuP();
    }
    sleep(1);
}

void turnoEnemigo(Clase *jugador, Enemigo *enemigo, FILE *logFile) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "\nTurno del %s\n", enemigo->nombre);
    enviarTexto(buffer);
    fprintf(logFile, "%s", buffer);
    sleep(1);

    int danyo = lanzar_dado(enemigo->veces, enemigo->ataque) - jugador->armadura;
    if (danyo < 0) danyo = 0;
    jugador->vida -= danyo;

    snprintf(buffer, sizeof(buffer), "El %s te ha hecho %d de dan\u00f5.\n", enemigo->nombre, danyo);
    enviarTexto(buffer);
    fprintf(logFile, "%s", buffer);
    sleep(1);
}

