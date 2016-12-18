#include <stdio.h>

int main(int args, char* argv[])
{
	for(int i=0; i<256; i++)
	{
		printf("\033[38;5;%dm\u25A0\u25A0\u25A0", i);
		if(i%16==0) printf("\n");
	}	

	printf("\033[1;0m\n");

	
	return 0;
}
