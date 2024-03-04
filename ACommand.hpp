#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

# include "Server.hpp"
# include "RPL.hpp"
# include "Client.hpp"
# include "ChannelMenager.hpp"
# include <map>

class Server;
class Client;

class ACommand {

protected:
	std::vector< std::string > command;

public:
	ACommand( void );
	ACommand( std::string & line );
	virtual ~ACommand( void );
	virtual void execute( std::string & line, Client & client, Server &server ) = 0;
	static void		splitMsgOnSpace( std::string &msg, std::vector< std::string > &tokens );
	static void		splitMsgOnComma( std::string &msg, std::vector< std::string > &tokens );
};

#endif