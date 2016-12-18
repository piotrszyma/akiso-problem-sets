/*
Program pozwala wysylac sygnaly do programu o danym PID
PID podajemy jako argument 1, a kolejne argumenty to numery
sygnalow, ktore chcemy wyslac

Co do pkt 1 zadania 2: Wysylajac z programu sender.c sygnaly
SYGKILL (id 9) program receiver zakonczy dzialanie niezaleznie od
stworzenia signalHandlera dla tego sygnalu. Nie jestesmy wiec
w stanie obsluzyc wszystkich sygnalow.

Co do pkt 3 zadania 2: Sygnaly nie sa kolejkowane. Uruchamiajc program
receiver i Wysylajac nastepnie
do procesu za pomoca tego programu sygnaly widzimy, ze
program nie otrzymuje ich w kolejnosci wyslania.

*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


int main(int args, char* argv[])
{

  for(int i=1; i < args - 1; i++) kill(atoi(argv[1]), atoi(argv[i+1]));

  return 0;
}
