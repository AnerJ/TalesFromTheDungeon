#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include "interfaz.h"

static SOCKET socketCliente = 0;

void setSocketCliente(SOCKET s) {
    socketCliente = s;
}

void getInput(char* buffer, int len) {
    if (socketCliente) {
        
        int bytes = recv(socketCliente, buffer, len - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
        } else {
            strcpy(buffer, "");
        }
    } else {
        fgets(buffer, len, stdin);
    }

    // Eliminar salto de línea
    size_t l = strlen(buffer);
    if (l > 0 && buffer[l - 1] == '\n') buffer[l - 1] = '\0';
}
void enviarTexto(const char* texto) {
    if (socketCliente) {
        printf("[DEBUG servidor] Enviando al cliente: %s", texto);

        send(socketCliente, texto, strlen(texto), 0);
    } else {
        printf("%s", texto);
    }
}
