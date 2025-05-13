#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main() {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return -1;
    }
    printf("Initialised.\n");

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (connect(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connection error: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return -1;
    }

    printf("Connected to server %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    // === Menú del jugador ===
    int opcion, claseID;
    char nombre[50], idStr[10];

    printf("1. Nueva partida\n2. Cargar partida\n3. Salir\n> ");
    scanf("%d", &opcion);
    getchar();  

    if (opcion == 1) {
        printf("Introduce nombre: ");
        fgets(nombre, sizeof(nombre), stdin);
        nombre[strcspn(nombre, "\n")] = 0;
        printf("Clase (1=Guerrero, 2=Cazador, 3=Pícaro): ");
        scanf("%d", &claseID);
        getchar();

        sprintf(sendBuff, "CREAR_PARTIDA;%s;%d", nombre, claseID);
    } else if (opcion == 2) {
        printf("Introduce ID del jugador: ");
        fgets(idStr, sizeof(idStr), stdin);
        idStr[strcspn(idStr, "\n")] = 0;
        sprintf(sendBuff, "CARGAR_PARTIDA;%s", idStr);
    } else {
        strcpy(sendBuff, "SALIR");
    }

    // Enviar comando al servidor
    send(s, sendBuff, sizeof(sendBuff), 0);
    recv(s, recvBuff, sizeof(recvBuff), 0);
    printf("Server response: %s\n", recvBuff);

    // Si no fue SALIR, seguir jugando (ejemplo: pedir mapa)
    if (strcmp(sendBuff, "SALIR") != 0) {
        strcpy(sendBuff, "MOSTRAR_MAPA");
        send(s, sendBuff, sizeof(sendBuff), 0);
        recv(s, recvBuff, sizeof(recvBuff), 0);
        printf("Server response: %s\n", recvBuff);

        // Enviar mensaje de despedida
        strcpy(sendBuff, "Bye");
        send(s, sendBuff, sizeof(sendBuff), 0);
        recv(s, recvBuff, sizeof(recvBuff), 0);
        printf("Server response: %s\n", recvBuff);
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
