#ifndef USER_HPP
# define USER_HPP

# include "ACommand.hpp"

class ACommand;

class USER : public ACommand {

private:

public:
	USER(void);
	USER( std::string &line );
	~USER( void );
	void execute( std::string & line, Client & client, Server &server );
};

#endif