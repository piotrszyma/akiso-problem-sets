#include <stdio.h>
#include <unistd.h>

int main(int args, char* argv[])
{
	printf("Proces 1\n");
	usleep(5000000);	
	return 0;
}
