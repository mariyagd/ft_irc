#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "ACommand.hpp"

class PRIVMSG : public ACommand {

public:
	PRIVMSG( void );
	~PRIVMSG( void );

	void execute( std::vector< std::string > & command, Client & client, Server &server );
	void send_in_channel( Server &server, Client & client, const std::string & message, const std::string & target );
	void send_to_client( Server & server, Client & client, const std::string & message, const std::string & target );


	};

#endif