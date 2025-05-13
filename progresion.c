#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "personaje.h"
#include "menus.h"
#include <winsock2.h>


void aumentoStats(Clase *p, int sala){
    char promt[10];
    int opcion;
    printf("Felicidades, valiente aventurero\n" );
    sleep(1);

    printf("Que tu poder crezca y tu determinacion no flaquee\n");
    sleep(1);
    printf("Elige una de estas tres bonificaciones:\n ");
    sleep(1);
    
    if (sala == 1){
        printf("1. +2 de Ataque\n 2. +2 de Armadura\n 3. +2 de Vida\n");
        
    } else if (sala == 2){
        printf("1. +3 de Ataque \n 2. +1 de Armadura \n 3. +5 de Vida\n");

    } else if (sala == 3){
        printf("1. +10 de Ataque \n 2. +3 de Armadura \n 3. +10 de Vida\n");

    } 

    printf("Selecciona una opcion introduciendo el numero: \n");
    fgets(promt, 10, stdin);

        //Eliminar el salto de linea
        if( promt[strlen(promt)-1] == '\n'){
            promt[strlen(promt)-1] = '\0';
        }

        //Pasar el valor a entero
    sscanf(promt, "%d", &opcion );


    if (sala == 1){
        if (opcion == 1){
            p->ataque += 2;

        } else if (opcion == 2){
            p->armadura+= 2;

        } else if (opcion == 3){
            p->vida += 2;
        } else if (opcion == 4){
            menuP();
        }



    } else if (sala == 2){
        if (opcion == 1){
            p->ataque += 3;

        } else if (opcion == 2){
            p->armadura+= 1;

        } else if (opcion == 3){
            p->vida += 5;
        } else if (opcion == 4){
            menuP();
        }

    } else if (sala == 3){
        if (opcion == 1){
            p->ataque += 10;

        } else if (opcion == 2){
            p->armadura+= 3;

        } else if (opcion == 3){
            p->vida += 10;
        } else if (opcion == 4){
            menuP();
        }
    }
}

void aumentoStatsOnline(Clase *p, int sala, SOCKET sock) {
    char sendBuff[512], recvBuff[64];
    int opcion;

    if (sala == 1)
        strcpy(sendBuff, "1. +2 Ataque\n2. +2 Armadura\n3. +2 Vida\n> ");
    else if (sala == 2)
        strcpy(sendBuff, "1. +3 Ataque\n2. +1 Armadura\n3. +5 Vida\n> ");
    else
        strcpy(sendBuff, "1. +10 Ataque\n2. +3 Armadura\n3. +10 Vida\n> ");

    send(sock, sendBuff, sizeof(sendBuff), 0);
    recv(sock, recvBuff, sizeof(recvBuff), 0);
    sscanf(recvBuff, "%d", &opcion);

    if (sala == 1) {
        if (opcion == 1) p->ataque += 2;
        else if (opcion == 2) p->armadura += 2;
        else if (opcion == 3) p->vida += 2;
    } else if (sala == 2) {
        if (opcion == 1) p->ataque += 3;
        else if (opcion == 2) p->armadura += 1;
        else if (opcion == 3) p->vida += 5;
    } else if (sala == 3) {
        if (opcion == 1) p->ataque += 10;
        else if (opcion == 2) p->armadura += 3;
        else if (opcion == 3) p->vida += 10;
    }
}
