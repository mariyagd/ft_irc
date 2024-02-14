#include <iostream>
#include "Server.hpp"

int	main( int ac, char **av )
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	try
	{
		Server	server(atoi(av[1]), av[2]);
	}
	catch (Server::ServerException &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
