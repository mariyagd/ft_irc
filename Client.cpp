#include "Client.hpp"

// Static member --------------------------------------------------------------------------------------------------------
// helps to close the server socket when the server is shut down

int Client::_serverSocket = -1;

// Coplien's form -------------------------------------------------------------------------------------------------------
Client::Client( void ) {

	_is_registered = false;
	_gave_password = false;
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
	_gave_password = false;
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

void	Client::setServerSocket( int socket ) {

	_serverSocket = socket;
	return;
}

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

	if ( !_nickname.empty() )
		_nickname.clear();

	_nickname = nick;
	return;
}

void	Client::setUsername( std::string user ) {

	if ( !_username.empty() )
		_username.clear();

	_username = user;
	return;
}

void	Client::setHostname( std::string hostname ) {

	if ( !_hostname.empty() )
		_hostname.clear();

	_hostname = hostname;
	return;
}

void	Client::setServname( std::string servname ) {

	_servname = servname;
	return;
}

void	Client::setRealname( std::string realname ) {

	if ( !_realname.empty() )
		_realname.clear();

	_realname = realname;
	return;
}

void	Client::setGavePassword( bool status ) {

	_gave_password = status;
	return;
}


// Getters --------------------------------------------------------------------------------------------------------------


bool	Client::getGavePassword( void ) const {

	return _gave_password;
}

int		Client::getSocket( void ) const {

	return _socket;
}

bool	Client::isRegistered( void ) const {

	return _is_registered;
}

std::string	Client::getNickname( void ) const {

	return _nickname;
}

std::string	Client::getUsername( void ) const {

	return _username;
}

std::string	Client::getHostname( void ) const {

	return _hostname;
}

std::string	Client::getServname( void ) const {

	return _servname;
}

std::string	Client::getRealname( void ) const {

	return _realname;
}

// Member functions -----------------------------------------------------------------------------------------------------

void	Client::cleanClient( void ) {

	_socket = -1;
	_is_registered = false;
	_gave_password = false;
	_nickname.clear();
	_username.clear();
	_hostname.clear();
	_servname.clear();
	_realname.clear();
	memset(&_addr, 0, sizeof( struct sockaddr ) );
	_addrlen = sizeof(_addr);
	return;
}

void	Client::closeSocket( void ) {

	int ret = 0;

	ret = close( _socket );
	if (ret < 0)
		std::cerr << PrintTime::printTime() << RED_BOLD << " --- Error while closing socket: " << strerror(errno) << END << std::endl;
	else
	{
		if ( _socket != _serverSocket )
			std::cout << PrintTime::printTime() << GREEN_BOLD << " --- Client [socket " << _socket << "] closed by server successfully" << END << std::endl;
		else
			std::cout << PrintTime::printTime() << GREEN_BOLD << " --- Server's [socket " << _socket << "] closed successfully" << END << std::endl;
	}
	cleanClient();
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