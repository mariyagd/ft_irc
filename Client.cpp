#include "Client.hpp"

// Coplien's form -------------------------------------------------------------------------------------------------------
Client::Client( void ) {

	_is_registered = false;
	_socket = -1;
	_name = "";
	_nick = "";
	_user = "";
	_realname = "";
	memset(&_addr, 0, sizeof( struct sockaddr ) );
	_addrlen = sizeof(_addr);
	return;
}

Client::~Client( void ) {

	return;
}

// Parameterized constructor --------------------------------------------------------------------------------------------

Client::Client( int socket ) : _socket(socket) {

	_is_registered = false;
	_socket = -1;
	_name = "";
	_nick = "";
	_user = "";
	_realname = "";
	memset(&_addr, 0, sizeof( struct sockaddr ) );
	_addrlen = sizeof(_addr);
	getpeername( _socket, &_addr, &_addrlen );
	return;
}

// Setters --------------------------------------------------------------------------------------------------------------

void	Client::setSocket( int socket ) {

	_socket = socket;
	_addrlen = sizeof(_addr);
	getpeername( _socket, &_addr, &_addrlen );

	std::cout << "Client address: " << inet_ntoa(((struct sockaddr_in *)&_addr)->sin_addr) << std::endl;

	fcntl(_socket, F_SETFL, O_NONBLOCK);
	return;
}

void	Client::setRegistered( bool status ) {

	_is_registered = status;
	return;
}

void	Client::setName( std::string name ) {

	_name = name;
	return;
}

void	Client::setNick( std::string nick ) {

	_nick = nick;
	return;
}

void	Client::setUser( std::string user ) {

	_user = user;
	return;
}

void	Client::setRealname( std::string realname ) {

	_realname = realname;
	return;
}

void	Client::setHostname( std::string hostname ) {

	_hostname = hostname;
	return;
}

// Getters --------------------------------------------------------------------------------------------------------------

int		Client::getSocket( void ) {

	return _socket;
}

bool	Client::isRegistered( void ) {

	return _is_registered;
}

std::string	Client::getName( void ) {

	return _name;
}

std::string	Client::getNick( void ) {

	return _nick;
}

std::string	Client::getUser( void ) {

	return _user;
}

std::string	Client::getRealname( void ) {

	return _realname;
}

std::string	Client::getHostname( void ) {

	return _hostname;
}

// Member functions -----------------------------------------------------------------------------------------------------

void	Client::closeSocket( void ) {

	int ret = 0;

	ret = close( _socket );
	if (ret < 0)
		std::cerr << "Error while closing socket: " << strerror(errno) << std::endl;
	else
		std::cout << "Socket " << _socket << " closed successfully" << std::endl;
	_socket = -1;
	return;
}