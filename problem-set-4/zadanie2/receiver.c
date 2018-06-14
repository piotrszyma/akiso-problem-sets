/*
Uruchamiajac program przy pomocy strace mozemy sledzic sygnaly,
jakie zostaly do niego przeslane z programu sender.c

Co do pkt 1 zadania 2: Wysylajac z programu sender.c sygnaly
SYGKILL (id 9) program zakonczy dzialanie niezaleznie od
stworzenia signalHandlera dla tego sygnalu. Nie jestesmy wiec
w stanie obsluzyc wszystkich sygnalow.

Co do pkt 3 zadania 2: Sygnaly nie sa kolejkowane. Uruchamiajc program
i Wysylajac nastepnie
do procesu za pomoca sender.c sygnaly widzimy, ze
program nie otrzymuje ich w kolejnosci wyslania.

*/


#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signalHandler(int);

int main(int args, char* argv[])
{
  printf("RECEIVER ID: %d\n", getpid() );

  for(int i = 1; i <= 64; i++) signal(i, signalHandler);

  while(1);

  return 0;

}

void signalHandler (int sig) {
  printf ("Received signal: %d \n", sig);
}
