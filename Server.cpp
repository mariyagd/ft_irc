
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

volatile std::sig_atomic_t 	Server::_shutdown_server = false;   // <------ static member initialization for signal handling

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
	_servSock = 0;
	_servinfo = 0;

	// initialize structures to 0
	memset( &_server_address, 0, sizeof( struct sockaddr_in ) );
	memset( &_hints, 0, sizeof( struct addrinfo ) );

	// initialize the connections table
	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
		_connections.push_back( Client() );
}

Server::~Server()
{
	for (int i = 0; i < MAX_CONNECTIONS; ++i)
	{
		if (_connections[i].getSocket() > 0)
			_connections[i].closeSocket();
	}
	if (_servinfo)
		freeaddrinfo(_servinfo);
	std::cout << BOLD << "Exit Server" << END << std::endl;
}


void Server::shutdown()
{
	for (int i = 0; i < MAX_CONNECTIONS; ++i)
	{
		if (_connections[i].getSocket() > 0)
			_connections[i].closeSocket();
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
		std::cout << std::endl << "Received SIGINT. Close program." << std::endl;
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
	std::cout << "Launching server on port " << _port << std::endl;

	sig_handler();

	get_addrinfo();
	socket();
	bind();
	listen();
	loop();
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

	int	status = ::bind( _servSock, reinterpret_cast<struct sockaddr*>(&_server_address), sizeof( _server_address ) );
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

	std::cout << BOLD << "Server socket fd " << _servSock << " is listening" << END << std::endl;
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
	FD_ZERO( &read_fd_set );
	FD_ZERO( &write_fd_set );

	for ( int i = 0; i < MAX_CONNECTIONS; ++i )											// set fd int the read_fd_set before passing it to the select call
	{
		if ( _connections[i].getSocket() >= 0 )
		{
			FD_SET( _connections[i].getSocket(), &read_fd_set );
			FD_SET( _connections[i].getSocket(), &write_fd_set );
		}
	}
}

void Server::accept( void )
{
	std::cout << "Server is ready to read" << std::endl;

	int clientSocket = ::accept(_servSock, reinterpret_cast<struct sockaddr *>( &new_addr ), &addrlen);
	if (clientSocket >= 0)
	{
		std::cout << "Accepted a new connection with fd: " << clientSocket << std::endl;
		for (int i = 0; i < MAX_CONNECTIONS; i++)                    // save the fd in the table
		{
			if (_connections[i].getSocket() < 0)
			{
				_connections[i].setSocket(clientSocket);
				return;
			}
		}
	}
	else
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
	return;
}

void	Server::receive( int i )
{
	int				ret = 0;
	const int		DATA_BUFFER =  5000;
	char			buf[DATA_BUFFER];
	std::string 	result = "";

	ret = recv( _connections[i].getSocket(), buf, DATA_BUFFER, 0 );
	if ( ret < 0 )
	{
		std::cerr << "recv() failed for fd: " << _connections[i].getSocket() << ": " << strerror(errno) << std::endl;
	}
	else if (ret == 0)
	{
		std::cout << "Connection " << _connections[i].getSocket() << " closed by client" << std::endl;
		_connections[i].closeSocket();
	}
	else
	{
		buf[ret] = '\0';
		result = buf;
		process(result, i);
	}
	FD_CLR(_connections[i].getSocket(), &read_fd_set);
	return;
}

void	Server::loop()
{
	int				ret = 0;

	_connections[0].setSocket( _servSock );												// set the first element of the table to the server socket

	while ( _shutdown_server == false )													// while there no SIGINT
	{
		reset_fds();
		ret = ::select( FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL );			// returns the number of connections ready to read
		if ( ret < 0 )
		{
			std::cerr << "Select failed: " << strerror( errno ) << std::endl;
			break;
		}
		for ( int i = 0; i < MAX_CONNECTIONS; i++ )
		{
			if (FD_ISSET(_connections[i].getSocket(), &read_fd_set))                                    // if server is ready to read, it will be in the read_fd_se
			{
				if (_connections[i].getSocket() == _servSock)
					accept();
				else
					receive(i);
			}
		}
	}
}

// Process received data ------------------------------------------------------------------------------------------------

void	Server::process_registration(std::string &msg, int i) {

	(void)i;
	std::string nick;
	std::string username;
	std::string hostname;
	std::string servname;
	std::string realname;
	std::string password;

	std::vector< std::string > tokens;

	std::istringstream iss(msg);
	while ( !iss.eof() )
	{
		std::string temp;
		iss >> temp;
		tokens.push_back(temp);
	}
	size_t j = 0;
	if ( tokens[j] == "PASS" )
	{
		password = tokens[++j];
		++j;
	}
	if ( tokens[j] == "NICK" )
	{
		nick = tokens[++j];
		++j;
	}
	if ( tokens[j] == "USER" )
	{
		username = tokens[++j];
		hostname = tokens[++j];
		servname = tokens[++j];
		for ( j += 1; j < tokens.size(); ++j )
		{
			realname += tokens[j] + " ";
		}
	}

	std::cout << "msg " << msg << std::endl;
	std::cout << "pass " << password << std::endl;
	std::cout << "nick " << nick << std::endl;
	std::cout << "username " << username << std::endl;
	std::cout << "hostname " << hostname << std::endl;
	std::cout << "servname " << servname << std::endl;
	std::cout << "realname " << realname << std::endl;
	if (password == _password)
	{
		_connections[i].setNick(nick);
		_connections[i].setUser(username);
		_connections[i].setRealname(realname);
		_connections[i].setHostname(hostname);
		_connections[i].setRegistered(true);
		std::cout << "Client " << _connections[i].getSocket() << " registered successfully" << std::endl;
	}
	else
	{
		std::cout << "Client " << _connections[i].getSocket() << " failed to register" << std::endl;
		send( _connections[i].getSocket(), "Error: wrong password\n", 22, 0 );
		_connections[i].closeSocket();
	}
}

void	Server::process( std::string &msg, int i )
{
	if (_connections[i].isRegistered() == false)
		process_registration(msg, i);
	else
		std::cout << "Process message: " << msg << std::endl;
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