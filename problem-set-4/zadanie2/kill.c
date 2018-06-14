#include <stdio.h>
#include <signal.h>

int main(void) {

  kill(1, 9);

  return 0;
}
