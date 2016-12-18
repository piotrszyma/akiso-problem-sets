#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

int main(int args, char* argv[])
{
	string line;
	getline(cin, line);
	cout<<line<<" + 2 proces";
	usleep(5000000);
	return 0;
}
