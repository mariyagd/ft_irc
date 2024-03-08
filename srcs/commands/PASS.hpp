#ifndef PASS_HPP
# define PASS_HPP

# include "ACommand.hpp"
# include "../Server.hpp"
# include "../Client.hpp"

//class ACommand;

class PASS : public ACommand {

private:

public:
	PASS( void );
	PASS( std::string &line );
	~PASS( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif