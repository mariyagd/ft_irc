#ifndef PART_HPP
# define PART_HPP

# include "ACommand.hpp"

class PART : public ACommand {

public:
	PART( void );
	~PART( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif