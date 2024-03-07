#ifndef SQUIT_HPP
# define SQUIT_HPP

#include "ACommand.hpp"

class SQUIT : public  ACommand {

public:
	SQUIT();
	~SQUIT();
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif