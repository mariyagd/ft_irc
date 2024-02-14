
#include "Server.hpp"

// Coplien's form

Server::Server(int port, char *password) : _port(port), _password(password)
{
	_socket_fd = 0;
	_hints = 0;
	_servinfo = 0;
}

Server::~Server()
{
	close(_socket_fd);
//	freeaddrinfo(_servinfo);
}

// socket

void	Server::socket()
{
	_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0 );
	if (_socket_fd < 0)
	{
		throw ServerException(strerror(errno));
	}
}

// launch

void	Server::launch()
{
 	// bind a socket
	_server_address.sin_family = AF_INET;						// for IPv4
	_server_address.sin_addr.s_addr = INADDR_ANY;
	_server_address.sin_port = htons(_port);

	bzero(_server_address.sin_zero, sizeof(_server_address.sin_zero));

	int	ret = bind(_socket_fd, (struct sockaddr *)&_server_address, sizeof(_server_address));
	if (ret < 0)
	{
		throw ServerException(strerror(errno));
	}

	ret = listen( _socket_fd, 10 );
	if (ret < 0)
	{
		throw ServerException(strerror(errno));
	}
}