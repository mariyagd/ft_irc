#include "Client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// Static member --------------------------------------------------------------------------------------------------------
// helps to close the server socket when the server is shut down

int Client::_id_num = 0;
int Client::_serverSocket = -1;

// used to get the client in chanel add const
/// @return
std::string Client::getnickname() const
{
	return _nickname;
}


// Coplien's form -------------------------------------------------------------------------------------------------------
Client::Client( void ) {

	_is_registered = false;
	_gave_password = false;
	_socket = -1;
	_id = -1;
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
	_id = -1;
	_username = "";
	_realname = "";
	memset(&_addr, 0, sizeof( struct sockaddr ) );
	_addrlen = sizeof(_addr);
	return;
}

// Setters --------------------------------------------------------------------------------------------------------------

void	Client::setServer( const int & socket ) {

	_socket = socket;
	_serverSocket = socket;
	return;
}

void	Client::setConnecion(const int & socket, const struct sockaddr & addr, const socklen_t & addrlen) {

	_socket = socket;
	_addr = addr;
	_addrlen = addrlen;

	int ret = 0;
	ret = fcntl(_socket, F_SETFL, O_NONBLOCK);
	if ( ret < 0 )
	{
		std::cerr << Get::Time() << RED_BOLD << " --- fcntl() failed: " << strerror(errno) << END << std::endl;
		closeSocket();
	}
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
//	_id = getNicknameId( );
	_id = ++_id_num;
	return;
}

void	Client::setUsername( std::string user ) {

	if ( !_username.empty() )
		_username.clear();

	_username = user.substr(0, 9);
	return;
}

// the name of the host machine of the user
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

const int &		Client::getSocket( void ) const {

	return _socket;
}

int	Client::getNicknameId( void ) const {

//	// Simple hash function to convert the string to an integer
//	int hash = 0;
//	for ( size_t i = 0; i < _nickname.length(); ++i ) {
//		hash = (hash * 31) + _nickname[i]; // A simple hash function
//	}
//	std::cout << "Nickname id from getNicknameId: " << hash << std::endl;
//	return hash;
	return _id;
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

int Client::getId( ) const {

	return _id;
}

// Bool------------------------------------------------------------------------------------------------------------------

bool	Client::isRegistered( void ) const {

	return _is_registered;
}


// Member functions -----------------------------------------------------------------------------------------------------

void	Client::cleanClient( void ) {

	_socket = -1;
	_id = -1;
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
		std::cerr << Get::Time() << RED_BOLD << " --- Error while closing socket: " << strerror(errno) << END << std::endl;
	else
	{
		if ( _socket == _serverSocket )
			std::cout << Get::Time() << GREEN_BOLD << " --- Server's [socket " << _socket << "] closed successfully" << END << std::endl;
		else
			std::cout << Get::Time() << GREEN_BOLD << " --- Client [socket " << _socket << "] closed by server successfully" << END << std::endl;
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


// Overload operator == ------------------------------------------------------------------------------------------------

bool	Client::operator==(const Client & rhs) const {

	if ( this->_nickname == rhs._nickname && this->_socket == rhs._socket )
		return true;
	return false;
}


// bool	Client::sendMessage( std::string & message ) {

// 	int ret;
// 	for ( size_t i = 0; i < message.length(); ++i )
// 	{
// 		if ( message[i] == '\n' )
// 			message[i] = '\r';
// 			ret = send( _socket, message.c_str(), message.length(), 0 );
// 	}
// 	if ( ret < 0 )
// 		std::cerr << Get::Time() << RED_BOLD << " --- Error while sending message: " << strerror(errno) << END << std::endl;
// 	else
// 		std::cout << Get::Time() << GREEN_BOLD << " --- Message sent successfully" << END << std::endl;
// 	return ( ret < 0 ? false : true );
// }967214913