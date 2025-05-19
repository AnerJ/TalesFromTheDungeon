// server.cpp
#include <iostream>
#include <winsock2.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

extern "C" {
#include "personaje.h"
#include "combate.h"
#include "datos.h"
#include "mazmorra.h"
#include "menus.h"
#include "progresion.h"
#include "interfaz.h"
#include "sqlite3.h"
#include "menus.h"
}

#define SERVER_PORT 6000

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    char recvBuf[512], sendBuf[512];

    std::cout << "Inicializando Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup falló: " << WSAGetLastError() << std::endl;
        return 1;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error creando socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Error en bind: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    listen(server_socket, 1);
    std::cout << "Esperando conexiones en el puerto " << SERVER_PORT << "...\n";

    int client_size = sizeof(client);
    client_socket = accept(server_socket, (struct sockaddr*)&client, &client_size);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Error en accept: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Cliente conectado.\n";
    setSocketCliente(client_socket);
    menuP();

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
