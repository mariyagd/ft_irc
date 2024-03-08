#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "ACommand.hpp"

class TOPIC : public ACommand {

	public:
	TOPIC(void);
	~TOPIC( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif