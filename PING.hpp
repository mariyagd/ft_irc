#ifndef PING_HPP
# define PING_HPP


# include "ACommand.hpp"
# include "Server.hpp"
# include "Client.hpp"
class PING : public ACommand {
	private:
	
	public:
		PING();
		PING( std::string &line );
		~PING( void );
		void execute( std::string & line, Client & client, Server &server );
};

#endif