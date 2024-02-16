
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
<<<<<<< HEAD
	_servSock = 0;
	memset( &_server_address, 0, sizeof( _server_address ) );	// make sure the struct is empty (i.e. all zeros
	bzero( &_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
=======
	_sockfd = 0;
	bzero(&_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
>>>>>>> ce0180308779777814eb3ef086c337f07222ad47
	_servinfo = 0;
}

Server::~Server()
{
<<<<<<< HEAD
	int ret = 0;

	for (int i = 0; i < MAX_CONNECTIONS; ++i)
	{
		if (all_connections[i] > 0)
		{
			ret = close(all_connections[i]);
			if (ret < 0)
				std::cerr << "Error while closing socket: " << strerror(errno) << std::endl;
			else
				std::cout << "Connection " << i << " closed successfully" << std::endl;

		}
	}

	ret = close(_servSock);
	if (ret < 0)
		std::cerr << "Error while closing socket: " << strerror(errno) << std::endl;
	else
		std::cout << "Server socket closed successfully" << std::endl;
	if (_servinfo)
		freeaddrinfo(_servinfo);
=======
	close(_sockfd);
//	freeaddrinfo(_servinfo);
>>>>>>> ce0180308779777814eb3ef086c337f07222ad47
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
<<<<<<< HEAD
=======
	// freeaddrinfo(_servinfo);
>>>>>>> ce0180308779777814eb3ef086c337f07222ad47
}

// Get addrinfo ----------------------------------------------------------------------------------------------------------

void	Server::get_addrinfo()
{
<<<<<<< HEAD

	bzero( &_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
	_hints.ai_family = AF_UNSPEC;						// don't care IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM;					// TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;						// fill in my IP for me

=======
	_hints.ai_family = AF_UNSPEC;						// don't care IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM;					// TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;						// fill in my IP for me

>>>>>>> ce0180308779777814eb3ef086c337f07222ad47
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
<<<<<<< HEAD

	_servSock = ::socket( PF_INET, SOCK_STREAM, 0 );
	if (_servSock < 0)
=======
	_sockfd = ::socket(AF_INET, SOCK_STREAM, 0 );
	if (_sockfd < 0)
>>>>>>> ce0180308779777814eb3ef086c337f07222ad47
	{
		std::string error_msg = "Socket error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout << "Socket fd " << _servSock << " created successfully " << std::endl;
}

// Bind ---------------------------------------------------------------------------------------------------------------

void	Server::bind()
{
<<<<<<< HEAD
	_server_address.sin_family = AF_INET;						// for IPv4
	_server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	_server_address.sin_port = htons( _port );

	bzero( _server_address.sin_zero, sizeof(_server_address.sin_zero ) );

	int	status = ::bind( _servSock, ( struct sockaddr * )&_server_address, sizeof( _server_address ) );
=======
 	// bind a socket
	_sockaddr.sin_family = AF_INET;						// for IPv4
	_sockaddr.sin_addr.s_addr = INADDR_ANY;
	_sockaddr.sin_port = htons(_port);

	bzero(_sockaddr.sin_zero, sizeof(_sockaddr.sin_zero));

	int	status = ::bind(_sockfd, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr));
>>>>>>> ce0180308779777814eb3ef086c337f07222ad47
	if (status < 0)
	{
		std::string error_msg = "Bind error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout	<< "Socket fd " << _servSock << " bound successfully."
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
<<<<<<< HEAD

	int	status = ::listen( _servSock, 10 );
=======
	int	status = ::listen( _sockfd, 10 );
>>>>>>> ce0180308779777814eb3ef086c337f07222ad47
	if (status < 0)
	{
		std::string error_msg = "Listen error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
<<<<<<< HEAD
=======
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
>>>>>>> ce0180308779777814eb3ef086c337f07222ad47

	std::cout << "Socket fd " << _servSock << " is listening" << std::endl;
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
	int				ret_val, i;
	const int		DATA_BUFFER =  5000;
	char			buf[DATA_BUFFER];

	/* Initialize all_connections and set the first entry to server fd */
	for ( i = 0; i < MAX_CONNECTIONS; ++i ) 
	{
		all_connections[i] = -1;
	}
	all_connections[0] = _servSock;

	while (1)
	{
		FD_ZERO( &read_fd_set );
		/* Set the fd_set before passing it to the select call */
		for ( i = 0; i < MAX_CONNECTIONS; ++i )
		{
			if ( all_connections[i] >= 0 )
			{
				FD_SET(all_connections[i], &read_fd_set);
			}
		}

		/* Invoke select() and then wait! */
		printf("\nUsing select() to listen for incoming events\n");

		ret_val = ::select( FD_SETSIZE, &read_fd_set, NULL, NULL, NULL );

		/* select() woke up. Identify the fd that has events */
		if (ret_val >= 0 )
		{
			printf("Select returned with %d\n", ret_val);
			/* Check if the fd with event is the server fd */
			if ( FD_ISSET( _servSock, &read_fd_set ) )
			{
				/* accept the new connection */
				printf("Returned fd is %d (server's fd)\n", _servSock);
				new_fd = ::accept( _servSock, ( struct sockaddr * )&new_addr, &addrlen );
				if (new_fd >= 0)
				{
					printf("Accepted a new connection with fd: %d\n", new_fd);
					for ( i = 0; i < MAX_CONNECTIONS; i++ )
					{
						if (all_connections[i] < 0)
						{
							all_connections[i] = new_fd;
							break;
						}
					}
				}
				else
				{
					fprintf(stderr, "accept failed [%s]\n", strerror(errno));
				}
				ret_val--;
				if ( !ret_val ) continue;
			}

			/* Check if the fd with event is a non-server fd */
			for ( i = 1; i < MAX_CONNECTIONS; ++i )
			{
				if ( ( all_connections[i] > 0 ) && ( FD_ISSET( all_connections[i], &read_fd_set ) ) )
				{
					/* read incoming data */
					printf("Returned fd is %d [index, i: %d]\n", all_connections[i], i);
					ret_val = recv( all_connections[i], buf, DATA_BUFFER, 0 );
					if ( ret_val == 0 )
					{
						printf("Closing connection for fd:%d\n", all_connections[i]);
						close( all_connections[i] );
						all_connections[i] = -1; /* Connection is now closed */
					}
					if (ret_val > 0)
					{
						printf("Received data (len %d bytes, fd: %d): %s\n", ret_val, all_connections[i], buf);
					}
					if (ret_val == -1)
					{
						printf("recv() failed for fd: %d [%s]\n", all_connections[i], strerror(errno));
						break;
					}
				}
				ret_val--;
				if ( !ret_val ) continue;
			} /* for-loop */
		} /* (ret_val >= 0) */
	} /* while(1) */

	/* Last step: Close all the sockets */

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