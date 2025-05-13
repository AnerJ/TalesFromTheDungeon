#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "combate.h"
#include "personaje.h"
#include "menus.h"
#include <unistd.h>
#include <winsock2.h>


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
    FILE *logFile = fopen("log/combate_log.txt", "a");
    if (logFile == NULL) {
        printf("Error al abrir el archivo de log.\n");
        return;
    }
    fprintf(logFile, "\n++++++++++++++++++++++++++++++\n");
    fprintf(logFile, "+  Enemigo: %s\n", enemigo->nombre);
    fprintf(logFile, "++++++++++++++++++++++++++++++\n");
    fprintf(logFile, "\n---------------------------------------------\n");

    printf("\n¡Un %s ha aparecido!\n", enemigo->nombre);
    printf("El combate comienza...\n");

    // Bucle del combate
    while (jugador->vida > 0 && enemigo->vida > 0) {
        sleep(1);
        printf("\n*****************\n");
        fprintf(logFile, "\n*****************\n");

        //Enseñamos las estadisticas
        printf(" Tus estadisticas               Estadisticas del Enemigo\n "
            "Vida: %d                       Vida: %d\n"
            " Ataque: %dd%d                   Ataque: %dd%d\n"
            " Armadura: %d                    Armadura: %d\n"
            " Velocidad: %d                   Velocidad: %d",
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
        int armaduraIncial = jugador->armadura;

        // Comprobar la velocidad para saber quien tiene turno primero
        if(jugador->velocidad >= enemigo->velocidad){
            printf("Eres mas rapido que tu rival\n");
            sleep(1);
            printf("Atacas primero\n");
            fprintf(logFile, "El jugador es mas rapido que el enemigo, ataca primero\n");
            sleep(1);
            turnoPersonaje(jugador, enemigo, logFile);
            if (enemigo->vida <= 0) {
                printf("¡Has derrotado al %s!\n", enemigo->nombre);
                fprintf(logFile, "\n++++++++++++++++++++++++++++++\n");
                fprintf(logFile, "*  %s derrotado\n", enemigo->nombre);
                fprintf(logFile, "++++++++++++++++++++++++++++++\n");
                fprintf(logFile, "\n---------------------------------------------\n");
                break;
            }
            turnoEnemigo(jugador, enemigo, logFile);

            if (jugador->vida <= 0) {
                printf("Has sido derrotado!\n");
                fprintf(logFile, "GAME OVER!\n");
                salir();
                break;
            }
            

        } else if (jugador->velocidad <= enemigo->velocidad){
            printf("El enemigo es mas rapido que tu\n");
            sleep(1);
            printf("Atacara primero\n");
            fprintf(logFile, "El enemigo es mas rapido que el jugador, el enemigo ataca primero\n");
            sleep(1);
            turnoEnemigo(jugador, enemigo, logFile);
            if (jugador->vida <= 0) {
                printf("Has sido derrotado!\n");
                fprintf(logFile, "GAME OVER!\n");
                salir();
                break;
            }
            turnoPersonaje(jugador, enemigo, logFile);
            if (enemigo->vida <= 0) {
                printf("¡Has derrotado al %s!\n", enemigo->nombre);
                fprintf(logFile, "\n++++++++++++++++++++++++++++++\n");
                fprintf(logFile, "*  %s derrotado\n", enemigo->nombre);
                fprintf(logFile, "++++++++++++++++++++++++++++++\n");
                fprintf(logFile, "\n---------------------------------------------\n");
                break;
            }

        }
        

        if (jugador->armadura != armaduraIncial){
            jugador->armadura = armaduraIncial;
        }

        // Fin del turno del enemigo
        fprintf(logFile, "\n---------------------------------------------\n");
    }

    fclose(logFile);
}

void turnoPersonaje(Clase *jugador, Enemigo *enemigo, FILE *logFile) {
    printf("\nTurno del jugador\n");
    fprintf(logFile, "\nTurno del jugador\n");
    sleep(1);
    
    printf("1. Atacar\n2. Defender\n");
    int opcion;
    char c[10];
    fgets(c, 10, stdin);
    if (c[strlen(c) - 1] == '\n') c[strlen(c) - 1] = '\0';

    sscanf(c, "%d", &opcion);

    if (opcion == 1) { // Ataque
        int danyo = lanzar_dado(jugador->veces, jugador->ataque) - enemigo->armadura;
        if (danyo < 0) danyo = 0;
        enemigo->vida -= danyo;
        system("cls");
        printf("Has hecho %d de daño al %s!\n", danyo, enemigo->nombre);
        fprintf(logFile, "Has hecho %d de daño al %s!\n", danyo, enemigo->nombre);
    } 
    else if (opcion == 2) { // Defensa
        jugador->armadura += 5;
        system("cls");
        printf("Vas a bloquear el siguiente ataque con %d de armadura\n", jugador->armadura);
        fprintf(logFile, "Bloqueo con %d de armadura\n", jugador->armadura);
    } else if (opcion == 4){
        menuP();
    }
    sleep(1);
}

void turnoEnemigo(Clase *jugador, Enemigo *enemigo, FILE *logFile) {
    printf("\nTurno del %s\n", enemigo->nombre);
    fprintf(logFile, "\nTurno del %s\n", enemigo->nombre);
    sleep(1);

    int danyo_enemigo = lanzar_dado(enemigo->veces, enemigo->ataque) - jugador->armadura;
    if (danyo_enemigo < 0) danyo_enemigo = 0;
    jugador->vida -= danyo_enemigo;

    printf("El %s te ha hecho %d de daño.\n", enemigo->nombre, danyo_enemigo);
    fprintf(logFile, "El %s te ha hecho %d de daño.\n", enemigo->nombre, danyo_enemigo);
    sleep(1);
}

void iniciarCombateOnline(Clase *jugador, Enemigo *enemigo, SOCKET sock) {
    char sendBuff[1024];
    FILE *logFile = fopen("log/combate_log.txt", "a");
    if (!logFile) return;

    snprintf(sendBuff, sizeof(sendBuff), "\n¡Un %s ha aparecido!\nEl combate comienza...\n", enemigo->nombre);
    send(sock, sendBuff, sizeof(sendBuff), 0);

    while (jugador->vida > 0 && enemigo->vida > 0) {
        snprintf(sendBuff, sizeof(sendBuff),
            "\nTu Vida: %d | Armadura: %d | Velocidad: %d\n"
            "Enemigo Vida: %d | Armadura: %d | Velocidad: %d\n",
            jugador->vida, jugador->armadura, jugador->velocidad,
            enemigo->vida, enemigo->armadura, enemigo->velocidad);
        send(sock, sendBuff, sizeof(sendBuff), 0);

        if (jugador->velocidad >= enemigo->velocidad) {
            send(sock, "\nAtacas primero\n", sizeof("\nAtacas primero\n"), 0);
            turnoPersonajeOnline(jugador, enemigo, logFile, sock);
            if (enemigo->vida <= 0) break;
            turnoEnemigoOnline(jugador, enemigo, logFile, sock);
        } else {
            send(sock, "\nEl enemigo ataca primero\n", sizeof("\nEl enemigo ataca primero\n"), 0);
            turnoEnemigoOnline(jugador, enemigo, logFile, sock);
            if (jugador->vida <= 0) break;
            turnoPersonajeOnline(jugador, enemigo, logFile, sock);
        }
    }
    fclose(logFile);
}

void turnoPersonajeOnline(Clase *jugador, Enemigo *enemigo, FILE *logFile, SOCKET sock) {
    char sendBuff[512], recvBuff[64];
    int opcion;

    strcpy(sendBuff, "\nTurno del jugador:\n1. Atacar\n2. Defender\n> ");
    send(sock, sendBuff, sizeof(sendBuff), 0);
    recv(sock, recvBuff, sizeof(recvBuff), 0);
    sscanf(recvBuff, "%d", &opcion);

    if (opcion == 1) {
        int danyo = lanzar_dado(jugador->veces, jugador->ataque) - enemigo->armadura;
        if (danyo < 0) danyo = 0;
        enemigo->vida -= danyo;
        snprintf(sendBuff, sizeof(sendBuff), "\nHas hecho %d de daño al %s\n", danyo, enemigo->nombre);
    } else {
        jugador->armadura += 5;
        snprintf(sendBuff, sizeof(sendBuff), "\nTe defiendes. Armadura +5 -> %d\n", jugador->armadura);
    }
    send(sock, sendBuff, sizeof(sendBuff), 0);
}

void turnoEnemigoOnline(Clase *jugador, Enemigo *enemigo, FILE *logFile, SOCKET sock) {
    char sendBuff[512];
    int danyo = lanzar_dado(enemigo->veces, enemigo->ataque) - jugador->armadura;
    if (danyo < 0) danyo = 0;
    jugador->vida -= danyo;
    snprintf(sendBuff, sizeof(sendBuff), "\nEl %s te ha hecho %d de daño.\n", enemigo->nombre, danyo);
    send(sock, sendBuff, sizeof(sendBuff), 0);
}