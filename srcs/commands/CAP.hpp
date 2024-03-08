#ifndef CAP_HPP
# define CAP_HPP

#include "ACommand.hpp"

class CAP : public ACommand {

public:
	CAP( void );
	~CAP( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif