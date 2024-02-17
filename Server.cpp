
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

volatile std::sig_atomic_t 	Server::_shutdown_server = false;   // <------ static member initialization for signal handling

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
	_servSock = 0;
	memset( &_server_address, 0, sizeof( _server_address ) );	// make sure the struct is empty (i.e. all zeros
	bzero( &_hints, sizeof( struct addrinfo ) );			// make sure the struct is empty
	_servinfo = 0;
}

Server::~Server()
{
	int ret = 0;

	for (int i = 0; i < MAX_CONNECTIONS; ++i)
	{
		if (all_connections[i] > 0)
		{
			ret = close( all_connections[i] );
			if (ret < 0)
				std::cerr << "Error while closing socket: " << strerror(errno) << std::endl;
			else
			{
				if (all_connections[i] == _servSock)
					std::cout << "Server socket closed successfully" << std::endl;
				else
					std::cout << "Connection fd " << all_connections[i]  << " closed successfully" << std::endl;
			}

		}
	}
//	ret = close(_servSock);
//	if (ret < 0)
//		std::cerr << "Error while closing socket: " << strerror(errno) << std::endl;
//	else
//		std::cout << "Server socket closed successfully" << std::endl;
	if (_servinfo)
		freeaddrinfo(_servinfo);
}


void Server::shutdown()
{
	int ret = 0;

	for (int i = 0; i < MAX_CONNECTIONS; ++i)
	{
		if (all_connections[i] > 0)
		{
			ret = close( all_connections[i] );
			if (ret < 0)
				std::cerr << "Error while closing socket: " << strerror(errno) << std::endl;
			else
				std::cout << "Connection fd " << all_connections[i]  << " closed successfully" << std::endl;

		}
	}
	if (_servinfo)
		freeaddrinfo(_servinfo);
	std::cout << "Exit Server" << std::endl;
	exit(0);
}

// Signal handling ------------------------------------------------------------------------------------------------------

void	Server::handler(int sig_code)
{
	if (sig_code == SIGINT)
	{
		std::cout << std::endl << "Received SIGINT." << std::endl;
		_shutdown_server = true;
	}
}

void	Server::sig_handler( void )
{
	struct sigaction	act;

	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);	/* Ctrl-\ */
	sigaddset(&act.sa_mask, SIGINT);	// Ctrl-C
	sigaddset(&act.sa_mask, SIGTSTP);	// Ctrl-Z
	act.sa_handler = SIG_IGN;			// ignore Ctrl-\, and Ctrl-Z
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	act.sa_handler = &handler;			// handle Ctrl-C
	sigaction(SIGINT, &act, 0);
}


// Launch ---------------------------------------------------------------------------------------------------------------

void	Server::launch()
{
	std::cout << "Launching server on port " << _port << std::endl;

	// signal handling
	sig_handler();

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

	_servSock = ::socket( PF_INET, SOCK_STREAM, 0 );
	if (_servSock < 0)
	{
		std::string error_msg = "Socket error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout << "Socket fd " << _servSock << " created successfully " << std::endl;
}

// Bind ---------------------------------------------------------------------------------------------------------------

void	Server::bind()
{
	_server_address.sin_family = AF_INET;						// for IPv4
	_server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	_server_address.sin_port = htons( _port );

	bzero( _server_address.sin_zero, sizeof(_server_address.sin_zero ) );

	int	status = ::bind( _servSock, ( struct sockaddr * )&_server_address, sizeof( _server_address ) );
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

	int	status = ::listen( _servSock, 10 );
	if (status < 0)
	{
		std::string error_msg = "Listen error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

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
	char *			keep_buf = NULL;
	std::string 	result = "";


	/* Initialize all_connections and set the first entry to server fd */
	for ( i = 0; i < MAX_CONNECTIONS; ++i ) 
	{
		all_connections[i] = -1;
	}
	all_connections[0] = _servSock;

	while ( _shutdown_server == false )		// <------------------------ while there no SIGINT
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
						buf[ret_val] = '\0';

						if ( strchr( buf, '\n' ) == NULL )
						{
							std::cout << "Data received. Found newline for ^D. Keep data for reconstitution " << std::endl;
							if (!keep_buf)
							{
								keep_buf = strdup( buf );
							}
							else
							{
								char *tmp = strdup( keep_buf );
								strcat( tmp, buf );
								free( keep_buf );
								keep_buf = strdup( tmp );
								free( tmp );
								tmp = NULL;
							}
							std::cout << "Keep in memory data \"" << keep_buf << "\"" << std::endl;

						}
						else
						{
							std::cout << "Data received. No newline found. Checking if there is data in memory." << std::endl;

							if ( keep_buf && buf[0] != '\0' )
							{
								std::cout << "Found data in memory. Reconstitution process" << std::endl;
								result = std::string(keep_buf);
							}
							else
							{
								result = std::string( buf );
							}
							std::cout << "Data (len " << ret_val << " bytes, fd: " << all_connections[i] << "): " << result << std::endl;
							if ( result == "exit\n" )
								return;
							if ( keep_buf )
							{
								free( keep_buf );
								keep_buf = NULL;
							}
							result.clear();
						}

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