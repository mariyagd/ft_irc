#ifndef QUIT_HPP
# define QUIT_HPP

#include "ACommand.hpp"

class QUIT : public ACommand {

public:
	QUIT( void );
	~QUIT( void );

	void execute( std::vector< std::string > & command, Client & client, Server & server );
};

#endif