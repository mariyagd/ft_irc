
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
	_sockfd = 0;
	bzero(&_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
	_servinfo = 0;
}

Server::~Server()
{
	close(_sockfd);
//	freeaddrinfo(_servinfo);
}

// Launch ---------------------------------------------------------------------------------------------------------------

void	Server::launch()
{
	get_addrinfo();
	socket();
	bind();
	listen();
	accept();
	// freeaddrinfo(_servinfo);
}

// Get addrinfo ----------------------------------------------------------------------------------------------------------

void	Server::get_addrinfo()
{
	_hints.ai_family = AF_UNSPEC;						// don't care IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM;					// TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;						// fill in my IP for me

	int	status = getaddrinfo(NULL, std::to_string(_port).c_str(), &_hints, &_servinfo);
	if (status != 0)
	{
		std::string error_msg = get_error_msg("Getaddrinfo error: ", gai_strerror(status) );
		throw ServerException( error_msg.c_str() );
	}
}

// Socket ---------------------------------------------------------------------------------------------------------------

void	Server::socket()
{
	_sockfd = ::socket(AF_INET, SOCK_STREAM, 0 );
	if (_sockfd < 0)
	{
		std::string error_msg = get_error_msg("Socket error: ", strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
}

// Bind ---------------------------------------------------------------------------------------------------------------

void	Server::bind()
{
 	// bind a socket
	_sockaddr.sin_family = AF_INET;						// for IPv4
	_sockaddr.sin_addr.s_addr = INADDR_ANY;
	_sockaddr.sin_port = htons(_port);

	bzero(_sockaddr.sin_zero, sizeof(_sockaddr.sin_zero));

	int	status = ::bind(_sockfd, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr));
	if (status < 0)
	{
		std::string error_msg = get_error_msg("Bind error: ", strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
}

// Listen ---------------------------------------------------------------------------------------------------------------

void	Server::listen()
{
	int	status = ::listen( _sockfd, 10 );
	if (status < 0)
	{
		std::string error_msg = get_error_msg("Listen error: ", strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
}

// accept ---------------------------------------------------------------------------------------------------------------

// Grab a connection from the queue
void	Server:: accept()
{
  socklen_t addrlen = sizeof(sockaddr);
  int connection = ::accept(_sockfd, (struct sockaddr*)&_sockaddr, (socklen_t*)&addrlen);
  if (connection < 0) {
    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  while (true) {
	
	// Read from the connection
	char buffer[100];
	read(connection, buffer, 100);
	std::cout << "The message was: " << buffer;

	if (strcmp(buffer, "EXIT\n") == 0) {
		std::cout << "EXIT received" << std::endl;
		break;
	}

	// Send a message to the connection
	std::string response = "Good talking to you\n";
	send(connection, response.c_str(), response.size(), 0);

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