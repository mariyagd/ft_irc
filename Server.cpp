
#include "Server.hpp"
# include "PASS.hpp"
# include "USER.hpp"
# include "NICK.hpp"
# include "WHOIS.hpp"
# include "PING.hpp"
# include "JOIN.hpp"
# include "MODE.hpp"
# include "PRIVMSG.hpp"
# include "KICK.hpp"
# include "INVITE.hpp"
# include "TOPIC.hpp"
# include "PART.hpp"
# include "WHO.hpp"
# include "SQUIT.hpp"
# include "CAP.hpp"
# include "QUIT.hpp"

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

	_command_executor.insert(std::make_pair("PASS", new PASS()));
	_command_executor.insert(std::make_pair("USER", new USER()));
	_command_executor.insert(std::make_pair("NICK", new NICK()));
	_command_executor.insert(std::make_pair("WHOIS", new WHOIS()));
	_command_executor.insert(std::make_pair("PING", new PING()));
	_command_executor.insert(std::make_pair("JOIN", new JOIN()));
	_command_executor.insert(std::make_pair("MODE", new MODE()));
	_command_executor.insert(std::make_pair("PRIVMSG", new PRIVMSG()));
	_command_executor.insert(std::make_pair("KICK", new KICK()));
	_command_executor.insert(std::make_pair("INVITE", new INVITE()));
	_command_executor.insert(std::make_pair("TOPIC", new TOPIC()));
	_command_executor.insert(std::make_pair("PART", new PART()));
	_command_executor.insert(std::make_pair("WHO", new WHO()));
	_command_executor.insert(std::make_pair("SQUIT", new SQUIT()));
	_command_executor.insert(std::make_pair("CAP", new CAP()));
	_command_executor.insert(std::make_pair("QUIT", new QUIT()));
}

Server::~Server()
{
	for ( int i = 0; i < MAX_CONNECTIONS + 1; ++i )
	{
		if ( _connections[i].getSocket() > 0 )
			_connections[i].closeSocket();
	}
	deleteAllChannels();
	for ( std::map< std::string, ACommand * >::iterator it = _command_executor.begin(); it != _command_executor.end(); it++ )
		delete it->second;
	
	std::cout << Get::Time() << BOLD << " --- Exit Server" << END << std::endl;
}

void Server::shutdown()
{
	for (int i = 0; i < MAX_CONNECTIONS + 1; ++i)
	{
		if (_connections[i].getSocket() > 0)
			_connections[i].closeSocket();
	}
	deleteAllChannels();
	for ( std::map< std::string, ACommand * >::iterator it = _command_executor.begin(); it != _command_executor.end(); it++ )
		delete it->second;

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

	_connections[0].setServer( _servSock, Get::HostMachineAddrInfo( *this ) );	// set the first element of the table to the server socket
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
				_connections[i].setConnecion( clientSocket, clientAddress /*, addrlen */ );
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
		RPL::RPL_ISUPPORT( client );
	}
}

void	Server::receive( int i )
{
	int				bytesRead = 0;
	char			buf[MSG_MAX_SIZE];

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
		_connections[i].setMessage( buf );

		std::cout << BLUE_BOLD << std::setw(100) << std::setfill('-') << "" << END << std::endl;
		if ( strchr (buf, '\n') == NULL )
		{
			std::cout << Get::Time() << ITALIC << " --- Received INCOMPLETE msg from [socket " << _connections[i].getSocket() << "] " << std::endl << CYAN_BOLD  << buf << END;
			std::cout << ITALIC << " --- Current state of the message: " << END << CYAN_BOLD << _connections[i].getCurrentMessage() << END << std::endl;
		}
		else
			std::cout << Get::Time() << " --- Received msg from [socket " << _connections[i].getSocket() << "] " << std::endl << CYAN_BOLD  << _connections[i].getCurrentMessage() << END;

		process_command(_connections[i].getMessage(), _connections[i] );

		if ( !_connections[i].isRegistered() )
			register_client(i);
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
int	Server::getSocketByNickname( const std::string &nickname ) const {

	for ( size_t i = 0; i < _connections.size(); ++i )
	{
		if ( _connections[i].getNickname() == nickname )
			return _connections[i].getSocket();
	}
	return -1;
}

//maybe put in a class Channel
Client *	Server::getClientByNickname( const std::string &nickname ) {

	size_t i = 0;
	for ( ; i < _connections.size(); ++i )
	{
		if ( _connections[i].getNickname() == nickname )
			return &_connections[i];
	}
	return nullptr;
}

void	Server::splitMsgOnSpace( std::string & line, std::vector< std::string > & tokens ) {

	std::istringstream iss( line );
	size_t pos = 0;

	while ( !iss.eof() )
	{
		std::string token;
		iss >> token;
		if ( (pos = token.find( "\n" )) != std::string::npos )
		{
			token.erase( pos, 1 );
		}
		if ( !token.empty() )
			tokens.push_back( token );
	}
	return ;
}

void Server::toUpper( std::string & command ) {

	if ( command.empty() )
		return ;
	if ( command[0] == '/' )
		command.erase(0, 1);
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
}

void	Server::process_command( const std::string & msg, Client & client )
{
//	std::cout << Get::Time() << " --- Processing message" << std::endl;

	if (msg.empty() || msg.find_first_of("\n") == std::string::npos )
		return;
	if (msg.find("CAP LS") == 0 )
	{
		const_cast< std::string & >( msg ) = msg.substr(0, 6);
	}
	std::istringstream iss(msg);

	while ( !iss.eof() )
	{
		std::vector< std::string > command;
		std::vector< std::string > tokens;
		std::string line;

		getline(iss, line, '\n' );
		if ( line.empty() )
			break;
		splitMsgOnSpace( line, tokens );
		toUpper( tokens[0] );

		std::map< std::string, ACommand * >::iterator it = _command_executor.find( tokens[0] );
		if ( it != _command_executor.end() )
			it->second->execute( tokens, client, *this );
		else if ( !client.isRegistered() )
			RPL::ERR_NOTREGISTERED( client );
		else
			RPL::ERR_UNKNOWNCOMMAND( client, tokens[0] );
	}
	return ;
}


void Server::print_all_info( void ) {


//	std::cout << std::endl << BLUE_BG << " --- Clients: " << END << std::endl;
	for (size_t i = 0; i < _connections.size(); ++i)
	{
		if ( _connections[i].getSocket() == _connections[i].getServerSocket() )
			continue;
		if (_connections[i].getSocket() >= 0)
		{
//			std::cout << " --- " << i << std::endl;
			_connections[i].printInfo( );
		}
	}
//	std::cout << std::endl << BLUE_BG << " --- Channels: " << END << std::endl;
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i] != nullptr)
		{
//			std::cout << " --- " << i << std::endl;
			_channels[i]->print_channels_info();
		}
	}
	std::cout << std::endl;

} // print_all_info
