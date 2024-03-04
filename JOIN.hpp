#ifndef JOIN_HPP
# define JOIN_HPP

# include "ACommand.hpp"

class JOIN : public ACommand {

public:
	JOIN( void );
	~JOIN( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif