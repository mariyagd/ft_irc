#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Server.hpp"
# include "RPL.hpp"
# include "Client.hpp"
# include "ChannelMenager.hpp"
# include <map>
//# include "ACommand.hpp"
# include "PASS.hpp"
//# include "USER.hpp"
//# include "NICK.hpp"

class Server;
class Client;
class ChannelMenager;
class ACommand;
class PASS;

class Commands {

private:
	Commands( void );

public:
	~Commands( void );
	static std::map< std::string, ACommand * > availableCommands;

	static void		splitMsgOnSpace( std::string &msg, std::vector< std::string > &tokens );
	static void		splitMsgOnComma( std::string &msg, std::vector< std::string > &tokens );
	static int		countNbCommands( std::vector< std::string > & tokens );
	static bool		isCommand( const std::string & token/*, std::vector< std::string > & availableCommands*/ );
	static void		initAvailableCommands( );
	static bool		is_unique_nickname(  std::string & nickname, int clientSocket, std::vector< Client > & connections );
	static void		concatenate( std::vector< std::string > & tokens, size_t i, std::string & msg );
	static std::map< std::string, void ( * )( std::vector< std::string > & , Client &, Server & ) > 	init_map( void );


	static void		choose_command( std::vector< std::string > & command, Client & client, Server &server );

	static void		process_command( std::string &msg, Client & client, Server & server );

//	static void 	PASS( std::vector< std::string > & command, Client & client, Server &server );
//	static void 	NICK( std::vector< std::string > & command, Client & client, Server &server );
//	static void 	USER( std::vector< std::string > & command, Client & client, Server &server );
//
//	static void 	WHOIS( std::vector< std::string > & command, Client & client, Server &server );
//	static void 	WHO( std::vector< std::string > & command, Client & client, Server &server );
//	static void 	PING( std::vector< std::string > & command, Client & client, Server &server );
//
//	static void 	JOIN( std::vector< std::string > & command, Client & client, Server &server );
//
//	static void		MODE( std::vector< std::string > & command, Client & client, Server & server );
//	static void 	setChannelMode( Client & client,  std::vector< std::string > & command, Channel * channel );
//	static bool		errorsInModeCommand( std::vector< std::string > & command, Client & client, Channel * channel );
//
//	static void		PRIVMSG( std::vector< std::string > & command, Client & client, Server & server );
//	static void 	KICK( std::vector< std::string > & command, Client & client, Server & server );
//	static void 	INVITE( std::vector< std::string > & command, Client & client, Server & server );
//	static void 	TOPIC( std::vector< std::string > & command, Client & client, Server & server );
//	static void 	PART( std::vector< std::string > & command, Client & client, Server & server );
////	static void 	CAP( std::vector< std::string > & command, Client & client, Server & server );

};

# endif