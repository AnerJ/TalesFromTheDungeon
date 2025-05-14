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

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define BUFFER 1024

int main() {
    WSADATA wsaData;
    SOCKET conn_socket, comm_socket;
    struct sockaddr_in server, client;
    char sendBuff[BUFFER], recvBuff[BUFFER];

    inicializarBD();      
    insertarClases();
    insertarEnemigos();

    printf("\nInicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Error al iniciar Winsock: %d\n", WSAGetLastError());
        return -1;
    }

    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    if (bind(conn_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error al hacer bind: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    listen(conn_socket, 1);
    printf("Esperando conexiones entrantes...\n");

    int client_size = sizeof(client);
    comm_socket = accept(conn_socket, (struct sockaddr*)&client, &client_size);
    if (comm_socket == INVALID_SOCKET) {
        printf("Error al aceptar conexión: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Cliente conectado desde %s:%d\n",
        inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Paso 1: Enviar menú
    strcpy(sendBuff,
        "\n=================================\n"
        "        TALES FROM THE DUNGEON\n"
        "=================================\n\n"
        "1. Nueva Partida -> CREAR_PARTIDA\n"
        "2. Cargar Partida -> CARGAR_PARTIDA;<idJugador>\n"
        "3. Salir -> SALIR\n\n> ");
    send(comm_socket, sendBuff, sizeof(sendBuff), 0);

    // Paso 2: Recibir comando
    memset(recvBuff, 0, sizeof(recvBuff));
    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

    // Paso 3: Interpretar comando
    if (strncmp(recvBuff, "CREAR_PARTIDA", 13) == 0) {
        char nombre[50];
        int claseID;

        // Solicitar nombre
        strcpy(sendBuff, "\n===============\n Creacion de Personaje\n===============\n\nDime tu nombre valiente aventurero:\n> ");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        sscanf(recvBuff, "%49[^\n]", nombre);

        // Solicitar clase
        strcpy(sendBuff,
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
            "Elige una clase:\n> ");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        sscanf(recvBuff, "%d", &claseID);

        Clase *pj = (Clase *)malloc(sizeof(Clase));
        strncpy(pj->nombre, nombre, sizeof(pj->nombre) - 1);
        pj->nombre[sizeof(pj->nombre) - 1] = '\0';
        pj->idJugador = insertarJugador(nombre, claseID);
        cargarClase(claseID, pj);
        pj->pos = 0;

        snprintf(sendBuff, sizeof(sendBuff),
            "\nPersonaje creado:\nNombre: %s\nClase ID: %d\nVida: %d\nArmadura: %d\nVelocidad: %d\nAtaque: %dd%d\n\n",
            pj->nombre, claseID, pj->vida, pj->armadura, pj->velocidad, pj->veces, pj->ataque);
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

        // Ejecutar partida online
        inciarPartidaOnline(pj, comm_socket);
    }
    else if (strncmp(recvBuff, "CARGAR_PARTIDA;", 15) == 0) {
        int id;
        sscanf(recvBuff, "CARGAR_PARTIDA;%d", &id);
        Clase *pj = (Clase *)malloc(sizeof(Clase));
        partidasCargadas(id, pj);

        snprintf(sendBuff, sizeof(sendBuff),
            "\nPartida cargada:\nNombre: %s\nID: %d\nSala: %d\nVida: %d\nArmadura: %d\nVelocidad: %d\nAtaque: %dd%d\n",
            pj->nombre, pj->idJugador, pj->pos, pj->vida, pj->armadura, pj->velocidad, pj->veces, pj->ataque);
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

        inciarPartidaOnline(pj, comm_socket);
    }
    else if (strcmp(recvBuff, "SALIR") == 0) {
        strcpy(sendBuff, "Gracias por jugar. ¡Hasta la próxima!\n");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
    }
    else {
        strcpy(sendBuff, "ERROR: Comando no reconocido.\n");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
    }

    // Cerrar conexión
    closesocket(comm_socket);
    cerrarBD();
    WSACleanup();
    return 0;
}
