
#include "Server.hpp"

// Coplien's form --------------------------------------------------------------------------------------------------------

volatile std::sig_atomic_t 	Server::_shutdown_server = false;   // <------ static member initialization for signal handling

Server::Server(int port, char *password) : _port(port), _password(password)
{
	(void)_password;
	_servSock = 0;
	_servinfo = 0;
	_server_name = "irc.lausanne42.ch";


	// initialize structures to 0
	memset( &_server_address, 0, sizeof( struct sockaddr_in ) );
	memset( &_hints, 0, sizeof( struct addrinfo ) );
	memset(&message, 0, MSG_MAX_SIZE);
	memset(&read_fd_set, 0, sizeof(fd_set));

	// initialize the connections table
	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
		_connections.push_back( Client() );
}


Server&		Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
	// Copy scalar members
		_port = rhs._port;
		_password = rhs._password;
		_servSock = rhs._servSock;

		// Deep copy _server_name
		// if (_server_name != nullptr) {
		// 	delete[] _server_name;
		// }
		// if (rhs._server_name != nullptr) {
		// 	_server_name = new char[strlen(rhs._server_name) + 1];
		// 	strcpy(_server_name, rhs._server_name);
		// } else {
		// 	_server_name = nullptr;
		// }

	}
	return (*this);
}

Server::~Server()
{
	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
	{
		if ( _connections[i].getSocket() > 0 )
			_connections[i].closeSocket();
	}
	if ( _servinfo )
		freeaddrinfo( _servinfo );

	std::cout << PrintTime::printTime() << BOLD << " --- Exit Server" << END << std::endl;
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
	std::cout << PrintTime::printTime() << BOLD << " --- Exit Server" << END << std::endl;
	exit(0);
}



// Signal handling ------------------------------------------------------------------------------------------------------

void	Server::handler(int sig_code)
{
	if (sig_code == SIGINT)
	{
		std::cout << std::endl << PrintTime::printTime() << BOLD << " --- Received SIGINT. Close program."  << END << std::endl;
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
	std::cout << PrintTime::printTime() << " --- Launching server on port " << _port << std::endl; // add time

	sig_handler();

	get_addrinfo();
	socket();
	bind();
	listen();
	loop();
}
// Get Current Time -----------------------------------------------------------------------------------------------------


// Get addrinfo ----------------------------------------------------------------------------------------------------------

void	Server::get_addrinfo()
{
	_hints.ai_family = PF_INET;						// don't care IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM;					// TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;						// fill in my IP for me
	_hints.ai_protocol = 0;								// any protocol

	int	status = getaddrinfo( NULL, std::to_string(_port).c_str(), &_hints, &_servinfo );
	if (status != 0)
	{
		std::string error_msg = PrintTime::printTime() + " Getaddrinfo error: " + std::string( gai_strerror(status) );
		throw ServerException( error_msg.c_str() );
	}
	std::cout << PrintTime::printTime() << " --- Getting address info successful." << std::endl;
}

// Socket ---------------------------------------------------------------------------------------------------------------

void	Server::socket()
{

	_servSock = ::socket( _servinfo->ai_family, _servinfo->ai_socktype , _servinfo->ai_protocol );
	if (_servSock < 0)
	{
		std::string error_msg = PrintTime::printTime() + " Server socket error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
	_connections[0].setSocket( _servSock );												// set the first element of the table to the server socket
	_connections[0].setServerSocket( _servSock );							    // set the static variable to keep track of the server socket
	std::cout << PrintTime::printTime() << " --- Server socket fd " << _servSock << " created successfully " << std::endl;
}

// Bind ---------------------------------------------------------------------------------------------------------------

void	Server::bind()
{
	_server_address.sin_family = PF_INET;						// for IPv4
	_server_address.sin_addr.s_addr = htonl( INADDR_ANY ); 		// let the system fill in the IP address
	_server_address.sin_port = htons( _port );

	bzero( _server_address.sin_zero, sizeof(_server_address.sin_zero ) );

	int	status = ::bind( _servSock, reinterpret_cast< struct sockaddr * >( &_server_address ), sizeof( _server_address ) );
	if (status < 0)
	{
		std::string error_msg = PrintTime::printTime()  + " --- Bind error: " + std::string( strerror(errno) );
		shutdown();  																				// close the server socket
		throw ServerException( error_msg.c_str() );
	}

	std::cout	<< PrintTime::printTime()
				<< " --- Server socket fd " << _servSock << " bound successfully. Protocol "
				<< getProtocolFamilyName(_server_address.sin_family)
				<< " Port "
				<< ntohs(_server_address.sin_port)
				<< " Address "
				<< inet_ntoa( _server_address.sin_addr ) << std::endl;
	return;
}

// Listen ---------------------------------------------------------------------------------------------------------------

void	Server::listen()
{

	int	status = ::listen( _servSock, 10 );
	if (status < 0)
	{
		std::string error_msg = PrintTime::printTime() + " --- Listen error: " + std::string( strerror(errno) );
		shutdown();																					// close the server socket
		throw ServerException( error_msg.c_str() );
	}
	std::cout << PrintTime::printTime() << GREEN_BOLD << " --- Server socket fd " << _servSock << " is listening" << END << std::endl;
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
//	FD_ZERO( &write_fd_set );

	for ( int i = 0; i < MAX_CONNECTIONS; ++i )											// set fd int the read_fd_set before passing it to the select call
	{
		if ( _connections[i].getSocket() >= 0 )
		{
			FD_SET( _connections[i].getSocket(), &read_fd_set );
//			FD_SET( _connections[i].getSocket(), &write_fd_set );
		}
	}
}


void Server::accept( void )
{
	std::cout << PrintTime::printTime() << " --- Server is ready to accept new connections" << std::endl;

	int clientSocket = ::accept(_servSock, reinterpret_cast<struct sockaddr *>( &new_addr ), &addrlen);
	if (clientSocket >= 0)
	{
		std::cout << PrintTime::printTime() << GREEN_BOLD << " --- Accepted a new connection [socket " << clientSocket << "]" << END << std::endl;
		for (int i = 0; i < MAX_CONNECTIONS; i++)                    // save the fd in the table, find available cell
		{
			if ( _connections[i].getSocket() < 0 )
			{
				_connections[i].setSocket( clientSocket );
				return;
			}
		}
		std::cout << RED_BOLD << PrintTime::printTime() << " --- Error: No room for new connection" << END << std::endl;
		// send a message to the client
		close( clientSocket );
	}
	else
		std::cout << PrintTime::printTime() <<  RED_BOLD << " --- Accept failed: " << strerror(errno) << END << std::endl;
	return;
}

void Server::register_client( int i )
{
	std::string 	message;
	Client & client = _connections[i];

	if ( client.getGavePassword() && !client.getNickname().empty() && !client.getUsername().empty() && !client.getHostname().empty() && !client.getRealname().empty() )
	{
		client.setRegistered( true );
		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " is now registered" << END << std::endl;

		client.printInfo();

		message = RPL::RPL_WELCOME( client, *this );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

		message = RPL::RPL_YOURHOST( client, *this );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

		message = RPL::RPL_CREATED( client, *this );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

		message = RPL::RPL_MYINFO( client, *this );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;
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
		std::cerr << RED_BOLD << PrintTime::printTime() << " --- recv() failed for fd: " << _connections[i].getSocket() << ": " << strerror(errno) << END << std::endl;
	}
	else if (bytesRead == 0)
	{
		std::cout << PrintTime::printTime() << " --- [socket " << _connections[i].getSocket() << "] left the IRC network" << std::endl;
		_connections[i].closeSocket();
	}
	else
	{
		buf[bytesRead] = '\0';
		msg = buf;
		std::cout << PrintTime::printTime() << CYAN_BOLD << " --- Received msg from [socket " << _connections[i].getSocket() << "] " << msg << END;

		Commands::process_command(msg, i, *this );
		if ( !_connections[i].isRegistered() )
			register_client(i);
	}
	FD_CLR(_connections[i].getSocket(), &read_fd_set);
	return;
}

void	Server::loop()
{
	int				ret = 0;

	while ( _shutdown_server == false )													// while there no SIGINT
	{
		reset_fds();
		ret = ::select( FD_SETSIZE, &read_fd_set, NULL, NULL, NULL );			// returns the number of connections ready to be read
		if ( ret < 0 )
		{
			std::cerr << PrintTime::printTime() << RED_BOLD << " --- Select failed: " << strerror( errno ) << END << std::endl;
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

char *	Server::getServerName( void ) const {

	return const_cast<char *>(_server_name.c_str());
}


// channel functions ------------------------------------------------------------------------------------------------------
Channel* Server::getChannel(const std::string& channelName)
{
	// Find the channel in the vector
	for (size_t i = 0; i < _channelName.size(); ++i) {
		if (_channelName[i]->getName() == channelName) {
			return _channelName[i]; // Return pointer to the Channel object
		}
	}
	return nullptr; // Return nullptr if channel not found
}

void Server:: addClientToChannel(const std::string& channelname, Client* client)
{
	for (size_t i = 0; i < _channelName.size(); ++i)
	{
		if (_channelName[i]->getName() == channelname)
		{
			_channelName[i]->addClient(client);
			std::cout<<"Added client "<< client->getNickname()<< " to channel "<<channelname<<std::endl;
			return;
		}
	}
	std::cerr<< "channel "<< channelname <<" not found. "<<std::endl;
}

void Server::addChannel(Channel *channel)
{
	_channelName.push_back(channel);
}

void	Server::sendToChannel(std::string&kickMessage, std::string& channel)
{
	 // Get the list of clients in the specified channel
    Channel* ch = getChannel(channel);
    if (ch) {
        std::vector<Client*> clients = ch->getClient();

        // Send the kick message to each client in the channel
        for (size_t i = 0; i < clients.size(); ++i) {
            clients[i]->sendMessage(kickMessage);
        }
    }
}


