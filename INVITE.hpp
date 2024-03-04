#ifndef INVITE_HPP
# define INVITE_HPP

# include "ACommand.hpp"

class INVITE : public ACommand {

public:
	INVITE( void );
	~INVITE( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif