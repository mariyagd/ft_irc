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

	static void send_message( const int & socket, const char * message, const size_t msg_size );

	static void REPLY( Client const & client, const std::set< int > & allClientsInAllChannels, const std::string & command, std::string & comment );

	// COMMON ERRORS
	static void ERR_NEEDMOREPARAMS( Client const & client, std::string const & command );

	// Registration
	static void RPL_WELCOME( Client const & client );
	static void RPL_YOURHOST( Client const & client );
	static void RPL_CREATED( Client const & client );
	static void RPL_MYINFO( Client const & client );

	static void ERR_ALREADYREGISTERED( Client const & client );
	static void ERR_PASSWDMISMATCH( Client const & client );

	// NICK
	static void ERR_NICKNAMEINUSE( Client const & client, std::string & wantedNickname );
	static void ERR_NONICKNAMEGIVEN( Client const & client );
	static void ERR_ERRONEUSNICKNAME( Client const & client );
	static void RPL_NICK( Client const & client, const std::set< int > & allClientsInAllChannels, std::string & newNickname );

	// WHOIS
	static void RPL_WHOISUSER( Client const & client );
	static void RPL_ENDOFWHOIS( Client const & client );
	static void RPL_ENDOFWHO( Client const & client, const std::string & channelName );

	// PING
	static void RPL_PING( Client const & client );
	static void ERR_NOORIGIN( Client const & client );

	//JOIN
	static void RPL_NAMREPLY( Client const & client, std::string & channelName, const std::vector< std::string > & operatorsNames );
	static void RPL_ENDOFNAMES( Client const & client, std::string & channelName );
	static void ERR_INVITEONLYCHAN( Client const & client, std::string & channelName );
	static void ERR_CHANNELISFULL( Client const & client, std::string & channelName );
	static void RPL_TOPIC( Client const & client, const std::string & channelName, const std::string & topic );
	static void RPL_TOPICWHOTIME( Client const & client, const Client & setter, const std::string & channelName, const long & creationTime );
	static void RPL_BADCHANNELKEY( Client const & client, std::string & channelName );

	// TOPIC
	static void RPL_NORMAL( Client const & client, const std::vector< Client * > & allClients, const std::string & channelName, const std::string & command, std::string & topic );
	static void RPL_NOTOPIC( Client const & client, std::string & channelName );


	//JOIN
	static void RPL_JOIN( Client const & client, const std::set< int > & allClientsInChannel, std::string & channelName );

	//CHANNEL MODE
	static void RPL_CHANNELMODEIS( Client const & client, const std::string & channelName, const std::string & allChannelModes );
	static void ERR_NOSUCHCHANNEL( Client const & client, std::string & channelName );
	static void RPL_CREATIONTIME( Client const & client, std::string & channelName, const long & creationTime );
	static void ERR_UNKNOWNMODE( Client const & client, const char & mode );
	static void INFORM_CHANNELMODE( Client const & client, const std::string  & channelName, const std::vector< std::string > & command, const std::vector< Client * > & allClients );


	// NICK MODE
	static void ERR_UMODEUNKNOWNFLAG( Client const & client );

	// PRIVMSG
	static void ERR_NORECIPIENT( Client const & client );
	static void ERR_NOTEXTTOSEND( Client const & client );
	static void ERR_NOSUCHNICK( Client const & client, std::string & nickname );
	static void RPL_PRIVMSG( Client const & client, std::string  nickname, std::string & message, const int & socket );
	static void RPL_PRIVMSG_CHANNEL( Client const & client, std::string & channelName, std::string & message );
	static void RPL_PRIVMSG_SELF( Client const & client, std::string & nickname, std::string & message );
	static void RPL_PRIVMSG_SELF_CHANNEL( Client const & client, std::string & channelName, std::string & message );
	static void ERR_CANNOTSENDTOCHAN ( Client const & client, std::string & channelName );

	// KICK
	static void ERR_CHANOPRIVSNEEDED( Client const & client, const std::string & channelName );
	static void RPL_KICK( Client const & client, std::string & channelName, std::string & nickname, std::string & comment, std::vector< Client * > allClients);
	static void ERR_NOTONCHANNEL( Client const & client, std::string & channelName );
	static void ERR_USERNOTINCHANNEL( Client const & client, std::string & nickname, std::string & channelName );

	static void RPL_MODE_OP(Client const & client, const std::string channelName, const std::string & currentChannelModes );

	// INVITE
	static void ERR_USERONCHANNEL( Client const & client, std::string & nickname, std::string & channelName );
	static void RPL_INVITING( Client const & client, Client const & invited, std::string & channelName );

	static void RPL_PART( Client const & client, const std::vector< Client *> allClients, const std::string & channelName, const std::string & comment );

};

#endif