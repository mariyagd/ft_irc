#include <iostream>
#include "Server.hpp"


int	main( int ac, char **av )
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	std::string port=av[1];

	if (port.find_first_not_of("0123456789", 0) != port.npos || std::atoi(av[1]) < 1024 || std::atoi(av[1]) > 65535)
	{
		std::cerr << "Error: port must be a number between 0 and 65535" << std::endl;
		return (1);
	}
	try
	{
		Server	server(atoi(av[1]), av[2]);
		server.launch();
	}
	catch (Server::ServerException &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
