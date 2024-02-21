#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Server.hpp"

class Server;
class Client;

class Commands {

private:
		Commands( void );
		Commands( Commands const & src );
		Commands & operator=( Commands const & rhs );
public:
	~Commands( void );
	static void	process_command( std::string &msg, int i, Server &server );
	static void 	NICK( std::string &nickname, int i, Server &server );

};

# endif