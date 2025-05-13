#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "personaje.h"
#include "datos.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main() {
    inicializarBD();
    insertarClases();
    insertarEnemigos();
    WSADATA wsaData;
    SOCKET conn_socket, comm_socket;
    struct sockaddr_in server, client;
    char sendBuff[512], recvBuff[512];

    printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return -1;
    }
    printf("Initialised.\n");

    // Crear socket
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    printf("Socket created.\n");

    // Configurar servidor
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    // Asociar socket
    if (bind(conn_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    printf("Bind done.\n");

    // Escuchar
    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Waiting for incoming connections...\n");
    int client_size = sizeof(struct sockaddr_in);
    comm_socket = accept(conn_socket, (struct sockaddr*)&client, &client_size);
    if (comm_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Connection accepted from: %s (%d)\n",
        inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Ya no necesitamos el socket de escucha
    closesocket(conn_socket);

    // Comunicación
    do {
        memset(recvBuff, 0, sizeof(recvBuff));
        int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (bytes > 0) {
            printf("Received: %s\n", recvBuff);

            // Procesar comandos
            if (strncmp(recvBuff, "CREAR_PARTIDA;", 14) == 0) {
                char nombre[50];
                int claseID;
                sscanf(recvBuff, "CREAR_PARTIDA;%49[^;];%d", nombre, &claseID);
            
                Clase *pj = (Clase *)malloc(sizeof(Clase));
                strcpy(pj->nombre, nombre);
                pj->idJugador = insertarJugador(nombre, claseID);
                cargarClase(claseID, pj);
                pj->pos = 0;
            
                snprintf(sendBuff, sizeof(sendBuff),
                    "OK;ID=%d;VIDA=%d;ARMA=%d;VEL=%d;VECES=%d;ATAQUE=%d",
                    pj->idJugador, pj->vida, pj->armadura, pj->velocidad, pj->veces, pj->ataque);
            
                free(pj);
            }
            else if (strncmp(recvBuff, "CARGAR_PARTIDA;", 15) == 0) {
                int idJugador;
                sscanf(recvBuff, "CARGAR_PARTIDA;%d", &idJugador);
            
                Clase *pj = (Clase *)malloc(sizeof(Clase));
                partidasCargadas(idJugador, pj);
            
                snprintf(sendBuff, sizeof(sendBuff),
                    "OK;ID=%d;POS=%d;VIDA=%d;ARMA=%d;VEL=%d;VECES=%d;ATAQUE=%d",
                    pj->idJugador, pj->pos, pj->vida, pj->armadura, pj->velocidad, pj->veces, pj->ataque);
            
                free(pj);
            }
            else if (strcmp(recvBuff, "SALIR") == 0) {
                strcpy(sendBuff, "ADIOS");
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                break;
            }
            else if (strcmp(recvBuff, "MOSTRAR_MAPA") == 0) {
                strcpy(sendBuff, "MAPA -> Estas en la sala 1");
            } else if (strcmp(recvBuff, "Bye") == 0) {
                strcpy(sendBuff, "ACK -> Adios");
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
                break;
            } else {
                strcpy(sendBuff, "ERROR -> Comando no reconocido");
            }

            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            printf("Sent: %s\n", sendBuff);
        }
    } while (1);

    closesocket(comm_socket);
    WSACleanup();
    return 0;
}
