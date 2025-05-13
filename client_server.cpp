#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define BUFFER 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    char sendBuff[BUFFER], recvBuff[BUFFER];

    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup falló. Error: %d\n", WSAGetLastError());
        return -1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("No se pudo conectar con el servidor: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    printf("Conectado al servidor %s:%d\n\n", SERVER_IP, SERVER_PORT);

    while (1) {
        memset(recvBuff, 0, BUFFER);
        int bytes = recv(sock, recvBuff, BUFFER - 1, 0);
        if (bytes <= 0) {
            printf("Conexión cerrada por el servidor.\n");
            break;
        }

        recvBuff[bytes] = '\0';
        printf("%s", recvBuff);
        fflush(stdout);

        if (strrchr(recvBuff, '>') != NULL) {
            printf("[CLIENTE] Prompt detectado. Esperando entrada...\n");
            memset(sendBuff, 0, BUFFER);
            fgets(sendBuff, BUFFER, stdin);
            sendBuff[strcspn(sendBuff, "\n")] = 0;
        
            if (send(sock, sendBuff, BUFFER, 0) == SOCKET_ERROR) {
                printf("Error al enviar datos al servidor.\n");
                break;
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
