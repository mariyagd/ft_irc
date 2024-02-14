
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
	_socket_fd = 0;
	_hints = 0;
	_servinfo = 0;
}

Server::~Server()
{
	close(_socket_fd);
//	freeaddrinfo(_servinfo);
}

// Launch ---------------------------------------------------------------------------------------------------------------

void	Server::launch()
{
	get_addrinfo();
	socket();
	bind();
	listen();
	freeaddrinfo(_servinfo);
}

// Get addrinfo ----------------------------------------------------------------------------------------------------------

void	Server::get_addrinfo()
{
	bzero(_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
	_hints->ai_family = AF_UNSPEC;						// don't care IPv4 or IPv6
	_hints->ai_socktype = SOCK_STREAM;					// TCP stream sockets
	_hints->ai_flags = AI_PASSIVE;						// fill in my IP for me

	int	status = getaddrinfo(NULL, std::to_string(_port).c_str(), _hints, &_servinfo);
	if (status != 0)
	{
		std::string error_msg = get_error_msg("Getaddrinfo error: ", strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
}

// Socket ---------------------------------------------------------------------------------------------------------------

void	Server::socket()
{
	_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0 );
	if (_socket_fd < 0)
	{
		std::string error_msg = get_error_msg("Socket error: ", strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
}

// Bind ---------------------------------------------------------------------------------------------------------------

void	Server::bind()
{
 	// bind a socket
	_server_address.sin_family = AF_INET;						// for IPv4
	_server_address.sin_addr.s_addr = INADDR_ANY;
	_server_address.sin_port = htons(_port);

	bzero(_server_address.sin_zero, sizeof(_server_address.sin_zero));

	int	status = ::bind(_socket_fd, (struct sockaddr *)&_server_address, sizeof(_server_address));
	if (status < 0)
	{
		std::string error_msg = get_error_msg("Bind error: ", strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
}

// Listen ---------------------------------------------------------------------------------------------------------------

void	Server::listen()
{
	int	status = ::listen( _socket_fd, 10 );
	if (status < 0)
	{
		std::string error_msg = get_error_msg("Listen error: ", strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
}

// Get error message -----------------------------------------------------------------------------------------------------

std::string Server::get_error_msg( std::string const error_msg1, char const *  error_msg2 ) const {

	std::string res = error_msg1 + error_msg2;
	return res;
}

// Exception handling-----------------------------------------------------------------------------------------------------

Server::ServerException::ServerException( const char *error_msg ) : _error_msg(error_msg) {

	return;
};


const char *Server::ServerException::what() const throw()
{
	return _error_msg;
}