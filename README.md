# TalesFromTheDungeon

Tales From the Dungeon es un juego de rol basado en un combate por turnos. El objetivo del juego es llegar a la 4 sala de la mazmorra la cual tendra una jefe final ha derrotar.
Con la derrota del enemigo final se finalizara el juego. 

# Reglas basicas del juego
El juego funciona a base de turnos y a base de unas estadisicas predefinidas que en el caso del jugador, son modificables. Estas estadisitcas son las siguientes y modifican el juego de la siguiente manera:


Velocidad -> Define que atacar primero en la ronda.


Cantidad de ataques posibles y Ataque ->Representado en el juego como 1d12, el primero numero es la cantidad de veces que se va atacar, y el siguiente numero a la d es la cantidad de daño que se puede infligir desde 1 al numero que pone en la estadistica que sera el maximo posible.
Armadura -> Mitiga el daño entrante, pero nunca baja
Vida -> Si los puntos de vida llegan a 0, el enemigo sera eliminado o el jugador perdera la partida
Una vez terminado cada combate, el jugador podra elegir una de tres bonificaciones para aumentarse las estadisticas anteriores.
El jugador tiene que elegir entre dos acciones posibles, atacar, ataca al enemigo, y defender, aumenta en 5 la cantidad de armadura del jugador para preparase para el siguiente ataque.
El juego guardara la partida despues de cada combate, es decir, una vez terminada la pelea se guardara partida y se podra cargar partida desde esa instancia. 
