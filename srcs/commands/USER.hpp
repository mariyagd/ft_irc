#ifndef USER_HPP
# define USER_HPP

# include "ACommand.hpp"


class USER : public ACommand {

public:
	USER(void);
	~USER( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif