#ifndef WHO_HPP

# define WHO_HPP

# include "ACommand.hpp"

class WHO : public ACommand {

public:
	WHO(void);
	~WHO( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif