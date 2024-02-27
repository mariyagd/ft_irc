#ifndef RPL_HPP
# define RPL_HPP

# include "Server.hpp"
# include <sys/time.h>


class Server;
class Client;

class RPL  {

private:
	RPL( void );

public:
	~RPL( void );

	static void send_message( const int socket, const char * message, const size_t msg_size );

	// Registration
	static void RPL_WELCOME( Client const & client );
	static void RPL_YOURHOST( Client const & client );
	static void RPL_CREATED( Client const & client );
	static void RPL_MYINFO( Client const & client );

	// NICK
	static void ERR_NICKNAMEINUSE( Client const & client, std::string & wantedNickname );
	static void ERR_NONICKNAMEGIVEN( Client const & client );
	static void ERR_ERRONEUSNICKNAME( Client const & client );
	static void RPL_SAVENICK( Client const & client );
	static void RPL_NICK( Client const & client, std::string & newNickname );

	// WHOIS
	static void RPL_WHOISUSER( Client const & client );
	static void ERR_NEEDMOREPARAMS( Client const & client, std::string const & command );

	// PING
	static void ERR_NOORIGIN( Client const & client );

	//JOIN
	static void RPL_NAMREPLY( Client const & client, std::string & channelName );
	static void RPL_ENDOFNAMES( Client const & client, std::string & channelName );

	//NORMAL
	static void RPL_JOIN( Client const & client, std::string & channelName );

	//CHANNEL MODE
	static void ERR_NOSUCHCHANNEL( Client const & client, std::string & channelName );
	static void RPL_CHANNELMODEIS( Client const & client, std::string & channelName );
	static void RPL_CREATIONTIME( Client const & client, std::string & channelName );

	// NICK MODE
	static void ERR_UMODEUNKNOWNFLAG( Client const & client );



};

#endif