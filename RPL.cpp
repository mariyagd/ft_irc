#include "RPL.hpp"

// Private constructor----------------------------------------------------------------------------------------------------

RPL::RPL( void ) {

	return;
}

// Destructor------------------------------------------------------------------------------------------------------------

RPL::~RPL( void ) {

	return;
}

// Send message-----------------------------------------------------------------------------------------------------------

void RPL::send_message( const int socket, const char * message, const size_t msg_size ) {

	int ret = 0;
	ret = send( socket, message, msg_size, 0 );
	if ( ret < 0 )
	{
		std::cerr << Get::Time() << RED << " --- send() failed" << strerror( errno ) << END << std::endl;
		return;
	}
	std::cout << Get::Time() << YELLOW_BOLD << " --- Send msg to [socket " << socket << "] " << message << END;
	return;
}

// Upon registration------------------------------------------------------------------------------------------------------


void	RPL::RPL_WELCOME( const Client & client ) {

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string message = "@time=" + Get::Time() + ":" + std::string(client.getServname());
	message += + " 001 " + client.getNickname()+ " Welcome to the Internet Relay Network " + msgto  + "\r\n";

	send_message( client.getSocket(), message.c_str(), message.size() );
}

void	RPL::RPL_YOURHOST( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 002 " + client.getNickname() + " Your host is " + client.getServname() + ", running version 1.0\r\n";
	send( client.getSocket(), message.c_str(), message.size(), 0 );
}

void	RPL::RPL_CREATED( Client const & client ) {

	std::string date = Get::Time().substr(0, 10);
	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 003 " + client.getNickname() + " This server was created " + date + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}


void	RPL::RPL_MYINFO( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 004 " + client.getNickname() + " " + client.getServname() + " v1.0 [user modes: none] [channel modes: itklo]\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

// For NICK -------------------------------------------------------------------------------------------------------------

//:tungsten.libera.chat 433 * mariyadancheva :Nickname is already in use.

void RPL::ERR_NICKNAMEINUSE( Client const & client, std::string & wantedNickname ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 433 " + wantedNickname + " " + wantedNickname + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::ERR_NONICKNAMEGIVEN( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 431 " + ":No nickname given\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::ERR_ERRONEUSNICKNAME( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 432 " + client.getNickname() + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::RPL_NICK( Client const & client, std::string & newNickname ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " NICK " + newNickname + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

// For WHOIS ------------------------------------------------------------------------------------------------------------

void RPL::RPL_WHOISUSER( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 311 " + client.getNickname() + " " + client.getUsername() + " " + client.getHostname() + " * :" + client.getRealname() + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::ERR_NEEDMOREPARAMS( Client const & client, std::string const & command ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 461 " + client.getNickname() + " " + command + ":Not enough parameters. Disconnecting\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

// PING PONG

void RPL::ERR_NOORIGIN( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 409 :No origin specified\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}


//JOIN
void  RPL::RPL_NAMREPLY(const Client &client, std::string &channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 353 " + client.getNickname() + " = " + channelName + " :" + client.getNickname() + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}


void  RPL::RPL_ENDOFNAMES(const Client &client, std::string &channelName) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

//NORMAL

void  RPL::RPL_JOIN(const Client &client, std::string &channelName) {

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string message = "@time=" + Get::Time() + ":" + msgto + " " + "JOIN " + channelName + " * " + client.getRealname() + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

// CHANNEL MODE

void RPL::ERR_NOSUCHCHANNEL( Client const & client, std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );

}

void  RPL::RPL_CHANNELMODEIS(const Client &client, std::string &channelName) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 324 " + client.getNickname() + " " + channelName + " +nt\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}
void  RPL::RPL_CREATIONTIME(const Client &client, std::string &channelName) {

	struct timeval tp;
	if ( gettimeofday( &tp, NULL ) < 0 )
	{
		std::cerr << RED << " --- gettimeofday() failed" << strerror( errno ) << END << std::endl;
		return;
	}
	long			stamp;
	stamp = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);



	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 329 " + client.getNickname() + " " + channelName + " " + std::to_string(stamp) + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

// NICK MODE
void RPL::ERR_UMODEUNKNOWNFLAG( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 501 " + client.getNickname() + " :Unknown MODE flag\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );

}

// PRIVMSG
void RPL::ERR_CANNOTSENDTOCHAN( Client const & client, std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 404 " + client.getNickname() + " " + channelName + " :Cannot send to channel\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}
