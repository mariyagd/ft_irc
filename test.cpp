#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int	 main(void )
{
	char welcome_msg[512];
	std::string nick = "test";
	std::string hostname = "localhost";
	sprintf(welcome_msg, ":Welcome in the IRC Connection !%s@%s\n", nick.c_str(), hostname.c_str());
	std::cout << welcome_msg << std::endl;
}