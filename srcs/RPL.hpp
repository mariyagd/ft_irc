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
	static void ERR_UNKNOWNCOMMAND( Client const & client, std::string const & command );
	static void ERR_USERONCHANNEL( Client const & client, std::string & nickname, std::string & channelName );
	static void ERR_BADCHANMASK( Client const & client, const std::string & channelName );
	static void ERR_NOSUCHCHANNEL( Client const & client, std::string & channelName );
	static void ERR_CHANOPRIVSNEEDED( Client const & client, const std::string & channelName );

	// Registration
	static void RPL_WELCOME( Client const & client );
	static void RPL_YOURHOST( Client const & client );
	static void RPL_CREATED( Client const & client );
	static void RPL_MYINFO( Client const & client );
	static void RPL_ISUPPORT( Client const & client );

	// Registration errors
	static void ERR_ALREADYREGISTERED( Client const & client );
	static void ERR_NOTREGISTERED( Client const & client );
	static void ERR_PASSWDMISMATCH( Client const & client );

	// NICK
	static void ERR_NICKNAMEINUSE( Client const & client, std::string & wantedNickname );
	static void ERR_NONICKNAMEGIVEN( Client const & client );
	static void ERR_ERRONEUSNICKNAME( Client const & client, const std::string & errorNickname );
	static void RPL_NICK( Client const & client, const std::set< int > & allClientsInAllChannels, std::string & newNickname );

	// WHOIS
	static void RPL_WHOISUSER( Client const & client, Client const & other );
	static void RPL_ENDOFWHOIS( Client const & client, const std::string & nick );
	static void RPL_ENDOFWHO( Client const & client, const std::string & channelName );
	static void RPL_WHOREPLY( Client const & client, const std::string & channelName );

	// PING
	static void RPL_PING( Client const & client );
	static void ERR_NOORIGIN( Client const & client );

	//JOIN
	// static void RPL_NAMREPLY( Client const & client, const std::string & channelName, const std::vector< std::string > & operatorsNames );
	static void RPL_NAMREPLY(const Client &client, const std::string &channelName, const std::vector<std::string> &operatorNames, const std::vector<std::string> &channelMembers);
	static void RPL_ENDOFNAMES( Client const & client, const std::string & channelName );
	static void ERR_INVITEONLYCHAN( Client const & client, const std::string & channelName );
	static void ERR_CHANNELISFULL( Client const & client, const std::string & channelName );
	static void RPL_BADCHANNELKEY( Client const & client, const std::string & channelName );

	// TOPIC
	static void RPL_TOPIC( Client const & client, const std::string & channelName, const std::string & topic );
	static void RPL_TOPICWHOTIME( Client const & client, const Client & setter, const std::string & channelName, const long & creationTime );
	static void RPL_NORMAL( Client const & client, const std::vector< Client * > & allClients, const std::string & channelName, const std::string & command, const std::string & topic );
	static void RPL_NOTOPIC( Client const & client, std::string & channelName );

	//JOIN
	static void RPL_JOIN( Client const & client, const std::set< int > & allClientsInChannel, const std::string & channelName );
	static void ERR_BADCHANNAME( Client const & client, const std::string & channelName );
	static void ERR_TOOMANYCHANNELS( Client const & client, const std::string & channelName );

	//CHANNEL MODE
	static void RPL_CHANNELMODEIS( Client const & client, const std::string & channelName, const std::string & allChannelModes );
	static void RPL_CREATIONTIME( Client const & client, std::string & channelName, const long & creationTime );
	static void ERR_UNKNOWNMODE( Client const & client, const char & mode );
	static void INFORM_CHANNELMODE( Client const & client, const std::string  & channelName, const std::vector< std::string > & command, const std::vector< Client * > & allClients );

	// NICK MODE
	static void ERR_UMODEUNKNOWNFLAG( Client const & client );
	static void RPL_ENDOFBANLIST ( Client const & client, const std::string & channelName) ;

	// PRIVMSG
	static void ERR_NORECIPIENT( Client const & client );
	static void ERR_NOTEXTTOSEND( Client const & client );
	static void ERR_NOSUCHNICK( Client const & client, const std::string & nickname );
	static void RPL_PRIVMSG( Client const & client,const  std::string  nickname, const std::string & message, const int & socket );
	static void ERR_CANNOTSENDTOCHAN ( Client const & client, const std::string & channelName );

	// KICK
	static void RPL_KICK( Client const & client, std::string & channelName, std::string & nickname, std::string & comment, const std::set< int > & allClientsInChannel );
	static void ERR_NOTONCHANNEL( Client const & client, std::string & channelName );
	static void ERR_USERNOTINCHANNEL( Client const & client, const std::string & nickname, const std::string & channelName );

	static void RPL_MODE_OP(Client const & client, const std::string channelName, const std::string & currentChannelModes );

	// INVITE
	static void RPL_INVITING( Client const & client, Client const & invited, std::string & channelName );

	// PART
	static void RPL_PART( Client const & client, const std::set< int > & allClientsInChannel, const std::string & channelName, const std::string & comment );

	// CAP
	static void RPL_CAP( Client const &client );
	static void ERR_INVALIDCAPCMD( Client const &client, const std::string &command );

	// QUIT
	static void QUIT( Client const & client, const std::set< int > & allClientsInAllChannels, const std::string & reason );

};

#endif