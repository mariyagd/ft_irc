#ifndef PASS_HPP
# define PASS_HPP

# include "ACommand.hpp"
# include "Server.hpp"
# include "Client.hpp"

//class ACommand;

class PASS : public ACommand {

private:

public:
	PASS( void );
	PASS( std::string &line );
	~PASS( void );
//	PASS( std::vector< std::string > &params, Client &client, Server &server );
	void execute( std::string & line, Client & client, Server &server );
};

#endif