
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

volatile std::sig_atomic_t 	Server::_shutdown_server = false;   // <------ static member initialization for signal handling

/*
 	 initialize the connections table
	 the first element of the table is the server socket
	 this is why we initialize the table with MAX_CONNECTIONS + 1 elements
 */

Server::Server(int port, char *password) : _port(port), _password(password) {

	memset(&_read_fd_set, 0, sizeof(fd_set));

	for ( int i = 0; i < MAX_CONNECTIONS + 1; ++i )
		_connections.push_back( Client() );
}

Server::~Server()
{
	for ( int i = 0; i < MAX_CONNECTIONS + 1; ++i )
	{
		if ( _connections[i].getSocket() > 0 )
			_connections[i].closeSocket();
	}
	std::cout << Get::Time() << BOLD << " --- Exit Server" << END << std::endl;
}

void Server::shutdown()
{
	for (int i = 0; i < MAX_CONNECTIONS + 1; ++i)
	{
		if (_connections[i].getSocket() > 0)
			_connections[i].closeSocket();
	}
	std::cout << Get::Time() << BOLD << " --- Exit Server" << END << std::endl;
}

// Signal handling ------------------------------------------------------------------------------------------------------

void	Server::handler(int sig_code)
{
	if (sig_code == SIGINT)
	{
		std::cout << std::endl << Get::Time() << BOLD << " --- Received SIGINT. Close program."  << END << std::endl;
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
	sigaddset(&act.sa_mask, SIGPIPE);	// client's deconnexion or crash
	act.sa_handler = SIG_IGN;			// ignore Ctrl-\, and Ctrl-Z
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGPIPE, &act, 0);
	act.sa_handler = &handler;			// handle Ctrl-C
	sigaction(SIGINT, &act, 0);
}

// Launch ---------------------------------------------------------------------------------------------------------------

void	Server::launch()
{
	std::cout << Get::Time() << " --- Launching server on port " << _port << std::endl; // add time

	sig_handler();

	socket();
	socket_options();
	bind();
	listen();
	loop();
}

// Socket ---------------------------------------------------------------------------------------------------------------

void	Server::socket()
{
	const int _servSock = ::socket( PF_INET, SOCK_STREAM , 0 );
	if (_servSock < 0)
	{
		std::string error_msg = Get::Time() + " Server socket error: " + std::string( strerror(errno) );
		shutdown();
		throw ServerException( error_msg.c_str() );
	}

	_connections[0].setServer( _servSock );	// set the first element of the table to the server socket
	std::cout << Get::Time() << GREEN_BOLD << " --- Server's socket created successfully [socket " << _servSock << "]" END << std::endl;

}

/*
 	 SO_REUSEADDR: Binding allowed (under certain conditions) to an address or port already in use
 	 il you close the server, and open it immediatly after that, you will be able to
	 instanciate again your server on the same port
 */

// Socket options --------------------------------------------------------------------------------------------------------

void	Server::socket_options()
{
	const int & _servSock = _connections[0].getSocket();
	int ret = 0;

	int		optval = 1;
	socklen_t	optval_lent = sizeof (optval);

	ret = getsockopt(_servSock, SOL_SOCKET, SO_REUSEADDR, &optval, &optval_lent);
	if ( ret < 0)
	{
		std::string error_msg = Get::Time() + " --- getsockopt() failed: " + std::string( strerror(errno) );
		shutdown();
		throw ServerException( error_msg.c_str() );
	}
	if ( optval == true ) // it means that SE_REUSEADDR is already set
		return;

	optval = 1;
	optval_lent = sizeof( optval );
	ret = setsockopt( _servSock, SOL_SOCKET, SO_REUSEADDR, &optval, optval_lent );
	if ( ret < 0 )
	{
		std::string error_msg = Get::Time() + " --- setsockopt() failed: " + std::string( strerror(errno) );
		shutdown();
		throw ServerException( error_msg.c_str() );
	}
}

// Bind ---------------------------------------------------------------------------------------------------------------

void	Server::bind()
{
	const int					_servSock = _connections[0].getSocket();
	struct sockaddr_in			_server_address;

	memset( &_server_address, 0, sizeof( struct sockaddr_in ) );
	memset( _server_address.sin_zero, 0, sizeof( _server_address.sin_zero ) );

	_server_address.sin_family = PF_INET;						// for IPv4
	_server_address.sin_addr.s_addr = htonl( INADDR_ANY ); 		// 0.0.0.0 wildcard, accept connections on any of the host's network addresses.
	_server_address.sin_port = htons( _port );

	int	status = ::bind( _servSock, reinterpret_cast< struct sockaddr * >( &_server_address ), sizeof( _server_address ) );

	if (status < 0 )
	{
		std::string error_msg = Get::Time()  + " --- error: " + std::string( strerror(errno) );
		shutdown();  // close the server socket
		throw ServerException( error_msg.c_str() );
	}
	if ( Get::Addrinfo() < 0 )
	{
		shutdown();
		throw ServerException( "Could not access IP adresses of the host machine" );
	}

	std::cout	<< Get::Time()
				<< " --- Server bound successfully to all available IP addresses" << std::endl
				<< "                         on his host machine that use protocol "
				<< BOLD << getProtocolFamilyName(_server_address.sin_family) << END
				<< " on port "
				<< BOLD << ntohs(_server_address.sin_port) << END << std::endl;

	return;
}

// Listen ---------------------------------------------------------------------------------------------------------------

void	Server::listen()
{
	int _servSock = _connections[0].getSocket();
	int	status = ::listen( _servSock, 10 );
	if (status < 0)
	{
		std::string error_msg = Get::Time() + " --- Listen error: " + std::string( strerror(errno) );
		shutdown();																					// close the server socket
		throw ServerException( error_msg.c_str() );
	}
	std::cout << Get::Time() << GREEN_BOLD << " --- Server is listening" << END << std::endl;
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

void Server::reset_fds( void )
{
	FD_ZERO( &_read_fd_set );

	for ( int i = 0; i < MAX_CONNECTIONS; ++i )											// set fd int the _read_fd_set before passing it to the select call
	{
		if ( _connections[i].getSocket() >= 0 )
		{
			FD_SET( _connections[i].getSocket(), &_read_fd_set );
		}
	}
}


void Server::accept( void )
{
	std::cout << Get::Time() << " --- Server is ready to accept new connections" << std::endl;

	int _servSock = _connections[0].getSocket();

	struct sockaddr clientAddress;
	memset(&clientAddress, 0, sizeof(struct sockaddr));
	socklen_t addrlen = sizeof(clientAddress);

	int clientSocket = ::accept( _servSock, &clientAddress, &addrlen );
	if (clientSocket >= 0)
	{
		std::cout << Get::Time() << GREEN_BOLD << " --- Accepted a new connection [socket " << clientSocket << "]" << END << std::endl;
		for (int i = 0; i < MAX_CONNECTIONS + 1; i++)                    // save the fd in the table, find available cell
		{
			if ( _connections[i].getSocket() < 0 )
			{
				_connections[i].setConnecion( clientSocket, clientAddress, addrlen );
				return;
			}
		}
		std::cout << RED_BOLD << Get::Time() << " --- Error: No room for new connection" << END << std::endl;
		close( clientSocket );
	}
	else
		std::cout << Get::Time() <<  RED_BOLD << " --- Accept failed: " << strerror(errno) << END << std::endl;
	return;
}

void Server::register_client( int i )
{
	std::string 	message;
	Client & client = _connections[i];

	if ( client.getGavePassword() && !client.getNickname().empty() && !client.getUsername().empty() && !client.getHostname().empty() && !client.getRealname().empty() )
	{
		client.setRegistered( true );
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " is now registered" << END << std::endl;

		client.printInfo();

		RPL::RPL_WELCOME( client );
		RPL::RPL_YOURHOST( client );
		RPL::RPL_CREATED( client );
		RPL::RPL_MYINFO( client );
	}
}

void	Server::receive( int i )
{
	int				bytesRead = 0;
	char			buf[MSG_MAX_SIZE];
	std::string 	msg = "";

	bytesRead = recv( _connections[i].getSocket(), buf, MSG_MAX_SIZE, 0 );
	if ( bytesRead < 0 )
	{
		std::cerr << RED_BOLD << Get::Time() << " --- recv() failed for fd: " << _connections[i].getSocket() << ": " << strerror(errno) << END << std::endl;
	}
	else if (bytesRead == 0)
	{
		std::cout << Get::Time() << " --- [socket " << _connections[i].getSocket() << "] left the IRC network" << std::endl;
		removeClientFromAllChannels( &_connections[i] );
		_connections[i].closeSocket();
	}
	else
	{
		buf[bytesRead] = '\0';
		msg = buf;
		std::cout << Get::Time() << CYAN_BOLD << " --- Received msg from [socket " << _connections[i].getSocket() << "] " << msg << END;

		Commands::process_command(msg, _connections[i], *this );
		if ( !_connections[i].isRegistered() )
			register_client(i);
		ChannelMenager::print_channels_info();
	}
	FD_CLR(_connections[i].getSocket(), &_read_fd_set);
	return;
}

void	Server::loop()
{
	int	ret = 0;
	int _servSock = _connections[0].getSocket();

	while ( _shutdown_server == false )													// while there no SIGINT
	{
		reset_fds();
		ret = ::select( FD_SETSIZE, &_read_fd_set, NULL, NULL, NULL );			// returns the number of connections ready to be read
		if ( ret < 0 )
		{
			if ( errno != EINTR ) // as we close the server only with ^C, select will return Interrupted system call. But this is not an error
			std::cerr << Get::Time() << RED_BOLD << " --- Select failed: " << strerror( errno ) << END << std::endl;
			break;
		}
		for ( int i = 0; i < MAX_CONNECTIONS; i++ )
		{
			if (FD_ISSET(_connections[i].getSocket(), &_read_fd_set))                                    // if server is ready to read, it will be in the read_fd_se
			{
				if (_connections[i].getSocket() == _servSock)
					accept();
				else
					receive(i);
			}
		}
	}
}

// Getters ------------------------------------------------------------------------------------------------------

std::string	Server::getProtocolFamilyName(int family) {
	if (family == AF_INET)
		return "IPv4";
	else if (family == AF_INET6)
		return "IPv6";
	else
		return "Unknown";
}

const std::string &	Server::getPassword( void ) const {

	return _password;
}

std::vector< Client > &	Server::getConnections( void ) {

	return _connections;
}

//maybe put in a class Channel
int	Server::getSocketByNickname( std::string &nickname ) {

	for ( size_t i = 0; i < _connections.size(); ++i )
	{
		if ( _connections[i].getNickname() == nickname )
			return _connections[i].getSocket();
	}
	return -1;
}

//maybe put in a class Channel
Client &	Server::getClientByNickname( std::string &nickname ) {

	size_t i = 0;
	for ( ; i < _connections.size(); ++i )
	{
		if ( _connections[i].getNickname() == nickname )
			break;
	}
	return _connections[i];
}