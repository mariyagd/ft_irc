
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
	_socket_fd = 0;
	bzero( &_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
	_servinfo = 0;
}

Server::~Server()
{
	int ret = 0;

	ret = close(_socket_fd);
	if (ret < 0)
	{
		std::cerr << "Error while closing socket: " << strerror(errno) << std::endl;
	}
	if (_servinfo)
		freeaddrinfo(_servinfo);
}

// Launch ---------------------------------------------------------------------------------------------------------------

void	Server::launch()
{
	std::cout << "Launching server on port " << _port << std::endl;

	get_addrinfo();
	socket();
	bind();
	listen();
	accept();
}

// Get addrinfo ----------------------------------------------------------------------------------------------------------

void	Server::get_addrinfo()
{

	bzero( &_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
	_hints.ai_family = AF_UNSPEC;						// don't care IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM;					// TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;						// fill in my IP for me

	int	status = getaddrinfo(NULL, std::to_string(_port).c_str(), &_hints, &_servinfo);
	if (status != 0)
	{
		std::string error_msg = "Getaddrinfo error: " + std::string( gai_strerror(status) );
		throw ServerException( error_msg.c_str() );
	}
	std::cout << "Getting address info successful." << std::endl;
}

// Socket ---------------------------------------------------------------------------------------------------------------

void	Server::socket()
{

	_socket_fd = ::socket( PF_INET, SOCK_STREAM, 0 );
	if (_socket_fd < 0)
	{
		std::string error_msg = "Socket error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout << "Socket fd " << _socket_fd << " created successfully " << std::endl;
}

// Bind ---------------------------------------------------------------------------------------------------------------

void	Server::bind()
{
	_server_address.sin_family = AF_INET;						// for IPv4
	_server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	_server_address.sin_port = htons( _port );

	bzero( _server_address.sin_zero, sizeof(_server_address.sin_zero ) );

	int	status = ::bind( _socket_fd, ( struct sockaddr * )&_server_address, sizeof( _server_address ) );
	if (status < 0)
	{
		std::string error_msg = "Bind error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout	<< "Socket fd " << _socket_fd << " bound successfully."
				<< " Familly "
				<< getProtocolFamilyName(_server_address.sin_family)
				<< " Port "
				<< ntohs(_server_address.sin_port)
				<< " Address "
				<< inet_ntoa(_server_address.sin_addr) << std::endl;
}

// Listen ---------------------------------------------------------------------------------------------------------------

void	Server::listen()
{

	int	status = ::listen( _socket_fd, 10 );
	if (status < 0)
	{
		std::string error_msg = "Listen error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout << "Socket fd " << _socket_fd << " is listening" << std::endl;
}


// Exception handling-----------------------------------------------------------------------------------------------------

Server::ServerException::ServerException( const char *error_msg ) : _error_msg(error_msg) {

	return;
};


const char *Server::ServerException::what() const throw()
{
	return _error_msg;
}

// Accept ---------------------------------------------------------------------------------------------------------------

void	Server::accept()
{
	int							client_socket_fd;
	struct sockaddr_storage		client_address;
	socklen_t					client_len;

	client_len = sizeof( client_address );
	client_socket_fd = ::accept( _socket_fd, ( struct sockaddr * )&client_address, &client_len );
	if (client_socket_fd < 0)
	{
		std::string error_msg = "Accept error: " + std::string( strerror(errno ) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout	<< "Server: got connection from "
				<< inet_ntoa( ( ( struct sockaddr_in * )&client_address )->sin_addr )
				<< " on socket fd " << client_socket_fd
				<< " Familly " << getProtocolFamilyName( ( ( struct sockaddr_in * )&client_address )->sin_family )
				<< std::endl;


	const char *password_request = "Please enter a password: \n";
	send( client_socket_fd, password_request, strlen( password_request ), 0);

	while ( true )
	{
		char buffer[100];
		int bytesRead = 0;
		bytesRead = recv( client_socket_fd, buffer, 100, 0 );
		if ( bytesRead <= 0 )
		{
			std::cerr << "Error while reading from socket: " << strerror(errno) << std::endl;
			break;
		}
		std::cout << "bytes  read: " << bytesRead << std::endl;
		buffer[ bytesRead - 1 ] = '\0';
		std::cout << "Got: " << buffer << std::endl;
		std::cout << "password expected: " << _password << std::endl;
		if ( strcmp( const_cast<char *>( buffer ) ,  _password ) == 0 )
		{
//			std::cout << "Connection established" << std::endl;
			send( client_socket_fd, "Connection established\n", strlen( "Connection established\n"), 0);

		}
		else
		{
//			std::cout << "Wrong password" << std::endl;
			send( client_socket_fd, "Wrong password\n", strlen( "Wrong password\n"), 0);

		}
	}
}



// Helper functions ------------------------------------------------------------------------------------------------------

std::string	Server::getProtocolFamilyName(int family)
{
	if (family == AF_INET)
		return "IPv4";
	else if (family == AF_INET6)
		return "IPv6";
	else
		return "Unknown";
}