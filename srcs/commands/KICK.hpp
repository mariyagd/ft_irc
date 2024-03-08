#ifndef KICK_HPP
#define KICK_HPP

#include "ACommand.hpp"

class KICK : public ACommand {

public:
	KICK( void );
	~KICK( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif