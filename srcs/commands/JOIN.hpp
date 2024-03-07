#ifndef JOIN_HPP
# define JOIN_HPP

# include "ACommand.hpp"

class JOIN : public ACommand {

public:
	JOIN( void );
	~JOIN( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
	bool modeRequirementsOK( const std::string & channelName, const std::vector< std::string > & keys, size_t & j, Channel * channel, Client & client );
	void join_and_send( Channel * channel, Client & client, const std::string & channelName );
};

#endif