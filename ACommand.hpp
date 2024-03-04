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

public:
	ACommand( void );

	virtual		~ACommand( void );
	virtual		void execute( std::vector< std::string > & command, Client & client, Server &server ) = 0;

	void		splitMsgOnComma( std::string &msg, std::vector< std::string > &tokens );
	void		concatenate( std::vector< std::string > & tokens, size_t i, std::string & msg );

	void		printVector( std::vector< std::string > & tokens );
};

#endif