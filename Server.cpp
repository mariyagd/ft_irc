
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

volatile std::sig_atomic_t 	Server::_shutdown_server = false;   // <------ static member initialization for signal handling

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
	_servSock = 0;
	_servinfo = 0;
	memset( &_server_address, 0, sizeof( _server_address ) );	// make sure the struct is empty (i.e. all zeros
	bzero( &_hints, sizeof( struct addrinfo ) );								// make sure the struct is empty

	// initialise for select()
	maxDescriptorPlus1 = -1;
	// Initialize all_connections
	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
		all_connections[i] = -1;
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

	bzero( &_hints, sizeof( struct addrinfo ) );		// make sure the struct is empty
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
		std::string error_msg = "Server socket error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}

	std::cout << "Server socket fd " << _servSock << " created successfully " << std::endl;
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

	std::cout	<< "Server socket fd " << _servSock << " bound successfully."
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

	std::cout << "Server socket fd " << _servSock << " is listening" << std::endl;
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
	int				ret, i;
	const int		DATA_BUFFER =  5000;
	char			buf[DATA_BUFFER];
	std::string 	result = "";


	// set the first entry to server fd
	all_connections[0] = _servSock;

	while ( _shutdown_server == false )										// while there no SIGINT
	{
		FD_ZERO( &read_fd_set );                                            // removes all descriptors from the vector

		for ( i = 0; i < MAX_CONNECTIONS; ++i )                            // set fd int the read_fd_set before passing it to the select call
		{
			if ( all_connections[i] >= 0 )
				FD_SET( all_connections[i], &read_fd_set );
		}

		ret = ::select( FD_SETSIZE, &read_fd_set, NULL, NULL, NULL );        // returns the number of connections ready to read

		if ( ret >= 0 )
		{
			if ( FD_ISSET( _servSock, &read_fd_set ) )                        // if server is ready to read, it will be in the read_fd_se
			{
				std::cout << "Server is ready to read" << std::endl;
				new_fd = ::accept( _servSock, ( struct sockaddr * ) &new_addr, &addrlen );
				if ( new_fd >= 0 )
				{
					std::cout << "Accepted a new connection with fd: " << new_fd << std::endl;
					for ( i = 0; i < MAX_CONNECTIONS; i++ )                    // save the fd in the table
					{
						if ( all_connections[i] < 0 )
						{
							all_connections[i] = new_fd;
							break;
						}
					}
				}
				else
					std::cerr << "Accept failed: " << strerror( errno ) << std::endl;
				ret--;
				if ( !ret ) continue;
			}

			for ( i = 1; i < MAX_CONNECTIONS; ++i )                            // start receiving data from all connections. index 0 is _serverSock
			{
				if ( ( all_connections[i] > 0 ) && ( FD_ISSET( all_connections[i], &read_fd_set ) ) )
				{
					ret = recv( all_connections[i], buf, DATA_BUFFER, 0 );
					if ( ret == 0 )
					{
						std::cout << "Connection " << all_connections[i] << " closed by client" << std::endl;
						close( all_connections[i] );
						all_connections[i] = -1;                            // delete the connection from the table
					}
					if ( ret > 0 )                                          // ^D handling starts here
					{
						buf[ret] = '\0';
						std::cout << "Received data \"" << buf << "\" from fd " << all_connections[i] << " ( len "
								  << strlen( buf ) << ")" << std::endl;

						if ( strchr( buf, '\n' ) == NULL )        // if there is no \n, there is ^D
						{
							std::cout << "Keep data in memory" << std::endl;
							result += std::string( buf );
						}
						else
						{
							std::cout << "Data is ready to be outputted" << std::endl;
							result += std::string( buf );
//							size_t pos = result.find( '\n' );					// to erase the final \n
//							result.erase(pos, 1);
							std::cout << "Output data: (len " << result.size( ) << ") \"" << result << "\"" << std::endl;
							if ( result == "exit\n" )
								return;
							result.clear( );
						}
					}
					if ( ret == -1 ) {
						std::cerr << "recv() failed for fd: " << all_connections[i] << ": " << strerror( errno )
								  << std::endl;
						break;
					}
				}
				ret--;
				if ( !ret ) continue;
			} /* for-loop */
		}/* (ret >= 0) */
	}/* while(1) */
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