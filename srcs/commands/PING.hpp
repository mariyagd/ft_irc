#ifndef PING_HPP
# define PING_HPP

# include "ACommand.hpp"
# include "../Server.hpp"
# include "../Client.hpp"

class PING : public ACommand {

public:
		PING();
		~PING( void );
		void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif