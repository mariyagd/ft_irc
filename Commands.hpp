#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Server.hpp"
# include "RPL.hpp"
# include "Client.hpp"
# include "ChannelMenager.hpp"

class Server;
class Client;
class ChannelMenager;

class Commands {

private:
	Commands( void );

public:
	~Commands( void );

	static void		splitMsgOnSpace( std::string &msg, std::vector< std::string > &tokens );
	static void		splitMsgOnComma( std::string &msg, std::vector< std::string > &tokens );
	static int		countNbCommands( std::vector< std::string > & tokens );
	static bool		isCommand( const std::string & token, std::vector< std::string > & availableCommands );
	static void 	deleteCommandFromList( std::string & token, std::vector< std::string > & availableCommands );
	static void		initAvailableCommands( std::vector< std::string > & availableCommands );
	static bool		is_unique_nickname(  std::string & nickname, int clientSocket, std::vector< Client > & connections );



	static void		choose_command( std::vector< std::string > & command, Client & client, Server &server );
	static void		process_command( std::string &msg, Client & client, Server & server );
	static void 	PASS( std::vector< std::string > & command, Client & client, Server &server );
	static void 	NICK( std::vector< std::string > & command, Client & client, Server &server );
	static void 	USER( std::vector< std::string > & command, Client & client, Server &server );

	static void 	WHOIS( std::vector< std::string > & command, Client & client, Server &server );
	static void 	PING( std::vector< std::string > & command, Client & client, Server &server );

	static void 	JOIN( std::vector< std::string > & command, Client & client, Server &server );
	static void			MODE( std::vector< std::string > & command, Client & client, Server & server );
	static void	 PRIVMSG( std::vector< std::string > & command, Client & client, Server & server );



};

# endif