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

void RPL::send_message( const int & socket, const char * message, const size_t msg_size ) {

	int ret = 0;
	ret = send( socket, message, msg_size, 0 );
	if ( ret < 0 )
	{
		std::cerr << Get::Time() << RED << " --- send() failed" << strerror( errno ) << END << std::endl;
		return;
	}
	std::cout << Get::Time() << " --- Send msg to [socket " << socket << "] " << std::endl  << YELLOW_BOLD << message << END;
	return;
}

void RPL::RPL_NORMAL( const Client & client, const std::vector< Client * > & allClients, const std::string & channelName, const std::string & command, std::string & comment ) {

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string message = "@time=" + Get::Time() + ":" + msgto + " " + command + " " + channelName + " " + comment + "\r\n";
	for ( size_t i = 0; i < allClients.size(); ++i )
	{
		send_message( allClients[i]->getSocket(), message.c_str(), message.size() );
	}
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

void RPL::ERR_ALREADYREGISTERED( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 462 " + client.getNickname() + " :You may not reregister\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::ERR_PASSWDMISMATCH( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 464 " + client.getNickname() + " :Password incorrect\r\n";
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

void RPL::RPL_NICK( Client const & client, const std::set< int > & allClientsInAllChannels, std::string & newNickname ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " NICK " + newNickname + "\r\n";
	for ( std::set< int >::const_iterator it = allClientsInAllChannels.begin(); it != allClientsInAllChannels.end(); it++ )
	{
		send_message( *it, message.c_str(), message.size() );
	}
}

void RPL::REPLY( Client const & client, const std::set< int > & allClientsInAllChannels, const std::string & command, std::string & newNickname ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " " + command + " " + newNickname + "\r\n";
	for ( std::set< int >::const_iterator it = allClientsInAllChannels.begin(); it != allClientsInAllChannels.end(); it++ )
	{
		send_message( *it, message.c_str(), message.size() );
	}
}

// For WHOIS ------------------------------------------------------------------------------------------------------------

void RPL::RPL_WHOISUSER( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 311 " + client.getNickname() + " " + client.getNickname() + " " + client.getUsername() + " " + client.getHostname() + " * " + client.getRealname() + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::RPL_ENDOFWHOIS( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 318 " + client.getNickname() + " :End of /WHOIS list\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::ERR_NEEDMOREPARAMS( Client const & client, std::string const & command ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 461 " + client.getNickname() + " " + command + ":Not enough parameters. Disconnecting\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::RPL_ENDOFWHO(const Client &client, const std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 315 " + client.getNickname() + " " + channelName + " :End of /WHO list.\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

// PING PONG

void	RPL::RPL_PING( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " PONG " + client.getServname() + " :" + client.getServname() +"\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::ERR_NOORIGIN( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 409 :No origin specified\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}


//JOIN
void  RPL::RPL_NAMREPLY(const Client &client, const std::string & channelName, const std::vector< std::string > & operatorNames ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 353 " + client.getNickname() + " = " + channelName + " :" + client.getNickname();
	for ( size_t i = 0; i < operatorNames.size(); ++i )
	{
		message += " @" + operatorNames[i];
		std::cout << CYAN_BG << operatorNames[i] << END << std::endl;
	}
	message += "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}


void  RPL::RPL_ENDOFNAMES(const Client &client, const std::string & channelName) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::ERR_INVITEONLYCHAN(const Client &client, const std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 473 " + client.getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}


void RPL::ERR_CHANNELISFULL( const Client &client, const std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 471 " + client.getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::RPL_TOPIC( const Client &client, const std::string &channelName, const std::string &topic ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 332 " + client.getNickname() + " " + channelName + " " + topic + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::RPL_NOTOPIC( const Client & client, std::string & channelName ) {

	std::string msg = "@time=" + Get::Time() + ":" + client.getServname() + " 331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n";
	send_message(client.getSocket(), msg.c_str(), msg.size() );
}

void RPL::RPL_TOPICWHOTIME( const Client &client, const Client & setter, const std::string &channelName, const long &creationTime ) {

	std::string msgto = setter.getNickname() + "!" + setter.getUsername() + "@" + setter.getServname();
	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 333 " + client.getNickname() + " " + channelName + " " + msgto + " " + std::to_string(creationTime) + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::RPL_BADCHANNELKEY( const Client &client, const std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 475 " + client.getNickname() + " " + channelName + " :Cannot join channel (+k)\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

//NORMAL

void	RPL::RPL_JOIN( Client const & client, const std::set< int > & allClientsInChannel, const std::string & channelName ) {

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string message = "@time=" + Get::Time() + ":" + msgto + " " + "JOIN " + channelName + " * " + client.getRealname() + "\r\n";
	for ( std::set< int >::const_iterator it = allClientsInChannel.begin(); it != allClientsInChannel.end(); it++ )
	{
		send_message( *it, message.c_str(), message.size() );
	}
}

// CHANNEL MODE

void RPL::ERR_NOSUCHCHANNEL( Client const & client, std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

/*
 * RPL_CHANNELMODEIS (324)
  "<client> <channel> <modestring> <mode arguments>..."
 Sent to a client to inform them of the currently-set modes of a channel.
 <channel> is the name of the channel. <modestring> and <mode arguments>
 are a mode string and the mode arguments (delimited as separate parameters)
 as defined in the MODE message description.
 */

void  RPL::RPL_CHANNELMODEIS(const Client &client, const std::string & channelName, const std::string & allChannelModes ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 324 " + client.getNickname() + " " + channelName + " " + allChannelModes + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}
void  RPL::RPL_CREATIONTIME(const Client &client, std::string &channelName, const long & creationTime) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 329 " + client.getNickname() + " " + channelName + " " + std::to_string(creationTime) + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void  RPL::ERR_UNKNOWNMODE(const Client &client, const char & mode) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 472 " + client.getNickname() + " " + mode + " :is unknown mode char to me for " + client.getServname() + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::INFORM_CHANNELMODE( Client const & client, const std::string  & channelName, const std::vector< std::string > & command, const std::vector< Client * > & allClients ) {

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string message = "@time=" + Get::Time() + ":" + msgto + " MODE " + channelName + " ";
	for ( size_t i = 0; i < command.size(); ++i )
	{
		message += command[i] + " ";
	}
	message += "\r\n";
	for ( size_t i = 0; i < allClients.size(); ++i )
	{
		RPL::send_message( allClients[i]->getSocket(), message.c_str(), message.size() );
	}
}


// NICK MODE
void RPL::ERR_UMODEUNKNOWNFLAG( Client const & client ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 501 " + client.getNickname() + " :Server doesn't support user modes\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );

}

// PRIVMSG
void RPL::ERR_CANNOTSENDTOCHAN( Client const & client, std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 404 " + client.getNickname() + " " + channelName + " :Cannot send to channel\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

// @time=2024-02-29T10:13:18.421Z :hkahsay__!~hkahsay@185.25.195.181 PRIVMSG #hamus :hi
/*
  :Angel PRIVMSG Wiz :Hello are you receiving this message ?
                                  ; Message from Angel to Wiz.

  :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
                                  ; Message from dan to the channel
                                  #coolpeople
*/
void RPL::RPL_PRIVMSG( const Client & client, std::string receiver, std::string & message, const int & socket ) 
{

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string msg = "@time=" + Get::Time() + ":" + msgto + " PRIVMSG " + receiver + " " + message + "\r\n";
	send_message( socket, msg.c_str(), msg.size() );
}

//  "<client> <nickname> :No such nick/channel"
void RPL::ERR_NOSUCHNICK( Client const & client, std::string & nickname ) {

	std::string msg = "@time=" + Get::Time() + ":" + client.getNickname() + " " + nickname + " :No such nick/channel" + "\r\n";
	send_message( client.getSocket(), msg.c_str(), msg.size() );
}

//kick<channel> <user>
void RPL::RPL_KICK( const Client & client, std::string & channelName, std::string & nickname, std::string & comment, const std::set< int > & allClientsInChannel ) {

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string message = "@time=" + Get::Time() + ":" + msgto + " KICK " + channelName + " " + nickname + " ";
	if (comment.empty())
		message += ":" + nickname + "\r\n";
	else
		message += comment + "\r\n";
	for ( std::set< int >::const_iterator it = allClientsInChannel.begin(); it != allClientsInChannel.end(); it++ )
	{
		send_message( *it, message.c_str(), message.size() );
	}
}

void RPL::ERR_CHANOPRIVSNEEDED( Client const & client, const std::string & channelName ) {

	std::string msg = "@time=" + Get::Time() + ":" + client.getServname() + " 482 " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n";
	send_message( client.getSocket(), msg.c_str(), msg.size() );
}

void RPL::ERR_NOTONCHANNEL( Client const & client, std::string & channelName ) {

	std::string msg = "@time=" + Get::Time() + ":" + client.getServname() + " 442 " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
	send_message( client.getSocket(), msg.c_str(), msg.size() );
}

void RPL::ERR_USERNOTINCHANNEL( Client const & client, std::string & nickname, std::string & channelName ) {

	std::string msg = "@time=" + Get::Time() + ":" + client.getServname() + " 441 " + client.getNickname() + " " + nickname + " " + channelName + " :They aren't on that channel\r\n";
	send_message( client.getSocket(), msg.c_str(), msg.size() );
}


void RPL::RPL_MODE_OP( const Client & client, const std::string channelName, const std::string & currentChannelModes ) {

	std::string msg = "@time=" + Get::Time() + ":" + client.getServname() + " MODE " + channelName + " " + currentChannelModes + "\r\n";
	send_message(client.getSocket(), msg.c_str(), msg.size() );
}


// INVITE

void RPL::ERR_USERONCHANNEL( const Client & client, std::string & nickname, std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 443 " + client.getNickname() + " " + nickname + " " + channelName + " :is already on channel\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );
}

void RPL::RPL_INVITING( const Client & client, Client const & invited, std::string & channelName ) {

	std::string message = "@time=" + Get::Time() + ":" + client.getServname() + " 341 " + client.getNickname() + " " + invited.getNickname() + " " + channelName + "\r\n";
	send_message( client.getSocket(), message.c_str(), message.size() );

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	message = "@time=" + Get::Time() + ":" + msgto + " INVITE " + invited.getNickname() + " " + channelName + "\r\n";
	send_message( invited.getSocket(), message.c_str(), message.size() );
}

// PART

void RPL::RPL_PART( const Client & client, const std::vector< Client *> allClients, const std::string & channelName, const std::string & comment ) {

	std::string msgto = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string message = "@time=" + Get::Time() + ":" + msgto + " PART " + channelName + " " + comment + "\r\n";
	for ( size_t i = 0; i < allClients.size(); ++i )
		send_message( allClients[i]->getSocket(), message.c_str(), message.size() );
}