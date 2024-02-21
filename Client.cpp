#include "Client.hpp"

// Coplien's form -------------------------------------------------------------------------------------------------------
Client::Client( void ) {

	_is_registered = false;
	_socket = -1;
	_nickname = "";
	_username = "";
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
	_nickname = "";
	_username = "";
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

	fcntl(_socket, F_SETFL, O_NONBLOCK);
	return;
}

void	Client::setRegistered( bool status ) {

	_is_registered = status;
	return;
}

void	Client::setNickname( std::string nick ) {

	_nickname = nick;
	return;
}

void	Client::setUsername( std::string user ) {

	_username = user;
	return;
}

void	Client::setHostname( std::string hostname ) {

	_hostname = hostname;
	return;
}

void	Client::setServname( std::string servname ) {

	_servname = servname;
	return;
}

void	Client::setRealname( std::string realname ) {

	_realname = realname;
	return;
}



// Getters --------------------------------------------------------------------------------------------------------------

int		Client::getSocket( void ) {

	return _socket;
}

bool	Client::isRegistered( void ) {

	return _is_registered;
}

std::string	Client::getNickname( void ) {

	return _nickname;
}

std::string	Client::getUsername( void ) {

	return _username;
}

std::string	Client::getHostname( void ) {

	return _hostname;
}

std::string	Client::getServname( void ) {

	return _servname;
}

std::string	Client::getRealname( void ) {

	return _realname;
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

// Print client info ----------------------------------------------------------------------------------------------------

void	Client::printInfo( void ) {

	std::cout << BLUE_BOLD << std::setw(50) << std::setfill('-') << "" << std::endl;
	std::cout << std::left << "Client info:"  << std::endl;
	std::cout << std::setw(50) << std::setfill('-') << "" << END << std::endl;

	std::cout << std::left << std::setw(15) << std::setfill(' ') << "Socket: " << _socket << std::endl;
	std::cout << std::left << std::setw(15) << "Registered: " << ( _is_registered == true ? "Yes" : "No" ) << std::endl;
	std::cout << std::left << std::setw(15) << "Nickname: " << _nickname << std::endl;
	std::cout << std::left << std::setw(15) << "Username: " << _username << std::endl;
	std::cout << std::left << std::setw(15) << "Hostname: " << _hostname << std::endl;
	std::cout << std::left << std::setw(15) << "Servname: " << _servname << std::endl;
	std::cout << std::left << std::setw(15) << "Realname: " << _realname << std::endl;
	std::cout << std::left << std::setw(15) << "Address: " << inet_ntoa(((struct sockaddr_in *)&_addr)->sin_addr) << std::endl;
	std::cout << BLUE_BOLD  << "------------------------------------------------------" << END << std::endl;
	return;
}