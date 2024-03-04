#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "ACommand.hpp"

class PRIVMSG : public ACommand {

public:
	PRIVMSG( void );
	~PRIVMSG( void );

	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif