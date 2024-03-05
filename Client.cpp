#include "Client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <curl/curl.h>

// Static member --------------------------------------------------------------------------------------------------------
// helps to close the server socket when the server is shut down

int Client::_id_num = 0;
int Client::_serverSocket = -1;
std::string Client::_serverName = "localhost";

// Coplien's form -------------------------------------------------------------------------------------------------------
Client::Client( void ) : _channels()  {

	_is_registered = false;
	_gave_password = false;
	_socket = -1;
	_id = -1;
	_nickname = "";
	_username = "";
	_realname = "";
//	memset(&_addr, 0, sizeof( struct sockaddr ) );
//	_addrlen = sizeof(_addr);
	return;
}

Client::~Client( void ) {

	return;
}

// Parameterized constructor --------------------------------------------------------------------------------------------

Client::Client( int socket ) : _socket(socket), _channels()  {

	_is_registered = false;
	_gave_password = false;
	_socket = -1;
	_nickname = "";
	_id = -1;
	_username = "";
	_realname = "";
//	memset(&_addr, 0, sizeof( struct sockaddr ) );
//	_addrlen = sizeof(_addr);
	return;
}

// Setters --------------------------------------------------------------------------------------------------------------

void	Client::setServer( const int & socket,  const std::string & serverName ) {

	_socket = socket;
	_serverSocket = socket;
	if ( !_serverName.empty() )
		_serverName = serverName;
	std::cout << CYAN_BG << serverName  << END << std::endl;
	return;
}

void	Client::setConnecion(const int & socket, const struct sockaddr & addr /*, const socklen_t & addrlen */) {

	_socket = socket;
//	_addr = addr;
//	_addrlen = addrlen;

	char clientIP[INET_ADDRSTRLEN];
	const char * hostname =  inet_ntop(AF_INET, &reinterpret_cast< const struct sockaddr_in * >( &addr )->sin_addr, clientIP, INET_ADDRSTRLEN);
	if ( !hostname )
	{
		std::cerr << Get::Time() << RED_BOLD << " --- inet_ntop() failed: " << strerror(errno) << END << std::endl;
		_hostname = "localhost";
	}
	else
		_hostname = std::string( hostname );

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
	_id = ++_id_num;
	return;
}

void	Client::setUsername( const std::string & username ) {

	if ( !_username.empty() )
		_username.clear();

	if ( username[0] == '~')
		_username = username.substr(0, 10);
	else
		_username = username.substr(0, 9);

	return;
}


// the name of the host machine of the user
//void	Client::setHostname( std::string hostname ) {
//
//	if ( !_hostname.empty() )
//		_hostname.clear();
//	_hostname = hostname;
//
//}
//
//void	Client::setServname( std::string servname ) {
//
//	_servname = servname;
//	return;
//}

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

	return _serverName;
}

std::string	Client::getRealname( void ) const {

	return _realname;
}

// Add and remove channels ----------------------------------------------------------------------------------------------

void	Client::addChannel( Channel * channel ) {

	if ( channel )
		_channels.push_back( channel );
	return;
}

void	Client::removeChannel( Channel * channel ) {

	if ( !channel )
		return;
	std::vector< Channel * >::iterator it = _channels.begin();
	for ( ; it != _channels.end(); it++ )
	{
		if ( *it == channel )
		{
			_channels.erase( it );
			break;
		}
	}
	return;
}

std::set< int > 	Client::getAllClientsInAllChannels( void ) const {

	std::set< int > users;
	users.insert( _socket );
	for ( size_t i = 0; i < _channels.size(); i++ )
	{
		std::set< int > temp = _channels[i]->getAllClientsSockets();
		for ( std::set< int >::iterator it = temp.begin(); it != temp.end(); it++ )
			users.insert( *it );
	}
	return users;
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
//	_servname.clear();
	_realname.clear();
//	memset(&_addr, 0, sizeof( struct sockaddr ) );
//	_addrlen = sizeof(_addr);
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

//	std::cout << BLUE_BOLD << std::setw(50) << std::setfill('-') << "" << std::endl;
	std::cout << std::setfill(' ');
	std::cout << std::left << std::setw(15) << BLUE_BOLD << "Client info:"  << END << std::endl;
//	std::cout << std::setw(50) << std::setfill('-') << "" << END << std::endl;

	std::cout << std::left << std::setw(15) << BLUE_BOLD << "Socket:     " << END << _socket << std::endl;
	std::cout << std::left << std::setw(15) << BLUE_BOLD << "Registered: " << END << ( _is_registered == true ? "Yes" : "No" ) << std::endl;
	std::cout << std::left << std::setw(15) << BLUE_BOLD << "Nickname:   " << END << _nickname << std::endl;
	std::cout << std::left << std::setw(15) << BLUE_BOLD << "Username:   " << END << _username << std::endl;
	std::cout << std::left << std::setw(15) << BLUE_BOLD << "Hostname:   " << END << _hostname << std::endl;
	std::cout << std::left << std::setw(15) << BLUE_BOLD << "Realname:   " << END << _realname << std::endl;

//	std::cout << BLUE_BOLD  << "------------------------------------------------------" << END << std::endl;
	return;
}

// Overload operator == ------------------------------------------------------------------------------------------------

bool	Client::operator==(const Client & rhs) const {

	if ( this->_socket == rhs._socket )
		return true;
	return false;
}

bool Client::operator!=(const Client & rhs) const {

	if ( this->_socket != rhs._socket )
		return true;
	return false;
}

int	Client::operator[](int) const {

	return _socket;
}

bool	Client::operator>=(int i) const {

	if ( _socket >= i )
		return true;
	return false;
}