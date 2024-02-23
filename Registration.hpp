#ifndef REGISTRATION_HPP
# define REGISTRATION_HPP

# include "Server.hpp"

class Server;

class Registration {

private:
	Registration( void );

public:
	~Registration( void );

	static void  splitMsgIntoTokens( std::string &msg, std::vector< std::string > &tokens );
	static bool ValidPassword( std::string &msg, Client & client, Server & server );
	static bool ClientReadyToRegister( Client const &client) ;
};

#endif