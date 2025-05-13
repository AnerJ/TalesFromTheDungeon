#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

extern "C" {
    #include "personaje.h"
    #include "datos.h"
    #include "combate.h"
    #include "mazmorra.h"
    #include "progresion.h"
    #include "menus.h"
}

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 6000
#define BUFFER      1024

int main() {
    WSADATA wsaData;
    SOCKET listen_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char sendBuf[BUFFER], recvBuf[BUFFER];

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Error al iniciar Winsock: %d\n", WSAGetLastError());
        return -1;
    }

    // Crear socket de escucha
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        printf("No se pudo crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port        = htons(SERVER_PORT);

    // Bind
    if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Error en bind: %d\n", WSAGetLastError());
        closesocket(listen_sock);
        WSACleanup();
        return -1;
    }

    // Escuchar
    listen(listen_sock, 1);
    printf("Esperando conexiones entrantes...\n");

    int client_len = sizeof(client_addr);
    client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock == INVALID_SOCKET) {
        printf("Error al aceptar conexión: %d\n", WSAGetLastError());
        closesocket(listen_sock);
        WSACleanup();
        return -1;
    }

    printf("Cliente conectado desde %s:%d\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    closesocket(listen_sock);

    // Inicializar BD y datos
    inicializarBD();
    insertarClases();
    insertarEnemigos();

    int running = 1;
    while (running) {
        // Menú principal
        strcpy(sendBuf,
            "\n=================================\n"
            "        TALES FROM THE DUNGEON\n"
            "=================================\n\n"
            "1. Nueva Partida -> CREAR_PARTIDA\n"
            "2. Cargar Partida -> CARGAR_PARTIDA;<idJugador>\n"
            "3. Salir -> SALIR\n\n> ");
        send(client_sock, sendBuf, strlen(sendBuf), 0);

        // Leer comando
        memset(recvBuf, 0, sizeof(recvBuf));
        if (recv(client_sock, recvBuf, sizeof(recvBuf)-1, 0) <= 0) break;

        // Proceso CREAR_PARTIDA
        if (strncmp(recvBuf, "CREAR_PARTIDA", 13) == 0) {
            char nombre[50];
            int claseID;

            // Paso 1: pedir nombre
            strcpy(sendBuf,
                "\n===============\n"
                " Creacion de Personaje\n"
                "===============\n\n"
                "Dime tu nombre valiente aventurero:\n> ");
            send(client_sock, sendBuf, strlen(sendBuf), 0);

            memset(recvBuf, 0, sizeof(recvBuf));
            recv(client_sock, recvBuf, sizeof(recvBuf)-1, 0);
            sscanf(recvBuf, "%49[^\n]", nombre);

            // Paso 2: mostrar clases
            snprintf(sendBuf, sizeof(sendBuf),
                "Valiente aventurero %s, Elige tu clase:\n\n"
                "*****************\n"
                "| 1. Guerrero   |\n"
                "| Ataque: 1d12  |\n"
                "| Armadura: 7   |\n"
                "| Vida: 20      |\n"
                "| Velocidad: 2  |\n"
                "*****************\n\n"
                "*****************\n"
                "| 2. Cazador    |\n"
                "| Ataque: 2d8   |\n"
                "| Armadura: 5   |\n"
                "| Vida: 15      |\n"
                "| Velocidad: 5  |\n"
                "*****************\n\n"
                "*****************\n"
                "| 3. Picaro     |\n"
                "| Ataque: 3d5   |\n"
                "| Armadura: 2   |\n"
                "| Vida: 13      |\n"
                "| Velocidad: 8  |\n"
                "*****************\n\n"
                "Elige una clase:\n> ", nombre);
            send(client_sock, sendBuf, strlen(sendBuf), 0);

            memset(recvBuf, 0, sizeof(recvBuf));
            recv(client_sock, recvBuf, sizeof(recvBuf)-1, 0);
            sscanf(recvBuf, "%d", &claseID);

            // Crear y registrar personaje
            Clase *pj = (Clase*)malloc(sizeof(Clase));
            strcpy(pj->nombre, nombre);
            pj->idJugador = insertarJugador(nombre, claseID);
            cargarClase(claseID, pj);
            pj->pos = 0;

            // Confirmación
            snprintf(sendBuf, sizeof(sendBuf),
                "\nPersonaje creado:\n"
                "Nombre: %s\n"
                "Clase ID: %d\n"
                "Vida: %d\n"
                "Armadura: %d\n"
                "Velocidad: %d\n"
                "Ataque: %dd%d\n\n",
                pj->nombre, claseID,
                pj->vida, pj->armadura,
                pj->velocidad,
                pj->veces, pj->ataque);
            send(client_sock, sendBuf, strlen(sendBuf), 0);

            // Iniciar aventura online
            inciarPartidaOnline(pj, client_sock);
            free(pj);
        }
        // Proceso CARGAR_PARTIDA
        else if (strncmp(recvBuf, "CARGAR_PARTIDA;", 15) == 0) {
            int id;
            sscanf(recvBuf, "CARGAR_PARTIDA;%d", &id);

            Clase *pj = (Clase*)malloc(sizeof(Clase));
            partidasCargadas(id, pj);

            snprintf(sendBuf, sizeof(sendBuf),
                "\nPartida cargada:\n"
                "Nombre: %s\n"
                "ID: %d\n"
                "Sala: %d\n"
                "Vida: %d\n"
                "Armadura: %d\n"
                "Velocidad: %d\n"
                "Ataque: %dd%d\n\n",
                pj->nombre, pj->idJugador,
                pj->pos, pj->vida, pj->armadura,
                pj->velocidad,
                pj->veces, pj->ataque);
            send(client_sock, sendBuf, strlen(sendBuf), 0);

            inciarPartidaOnline(pj, client_sock);
            free(pj);
        }
        // Salir
        else if (strncmp(recvBuf, "SALIR", 5) == 0) {
            strcpy(sendBuf, "Gracias por jugar. ¡Hasta la próxima!\n");
            send(client_sock, sendBuf, strlen(sendBuf), 0);
            running = 0;
        }
        // Comando desconocido
        else {
            strcpy(sendBuf, "ERROR: Comando no reconocido.\n");
            send(client_sock, sendBuf, strlen(sendBuf), 0);
        }
    }

    // Limpieza final
    closesocket(client_sock);
    cerrarBD();
    WSACleanup();
    return 0;
}
