#include <iostream>
#include <winsock2.h>
#include <string>
#include <cstring>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main() {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char recvBuff[512];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return 1;

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) return 1;

    
    u_long modo = 1;
    ioctlsocket(s, FIONBIO, &modo);

    std::string comando;

    while (true) {
        // 1. Mostrar todo lo que llega del servidor
        int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
        if (bytes > 0) {
            recvBuff[bytes] = '\0';
            std::cout << recvBuff << std::flush;
        } else if (bytes == 0) {
            std::cout << "\n[INFO] El servidor cerró la conexión.\n";
            break;
        }else {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) {
                // No pasa nada, no hay datos disponibles todavía
            } else if (error == WSAECONNRESET) {
                std::cerr << "\nConexion cerrada por el servidor.\n";
                break;
            } else {
                std::cerr << "\n[ERROR] Error inesperado en recv(): " << error << "\n";
                break;
            }
        }


        // 2. Si el usuario pulsa Enter, leer input
        if (_kbhit()) {
            std::getline(std::cin, comando);
            if (!comando.empty()) {
                send(s, comando.c_str(), comando.size() + 1, 0);
                if (comando == "salir"){
                    exit(0);
                    break;
                }
                
            }
        }

        Sleep(10);  
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
