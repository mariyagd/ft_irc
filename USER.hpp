#ifndef USER_HPP
# define USER_HPP

# include "ACommand.hpp"
# include "Server.hpp"
# include "Client.hpp"


class USER : public ACommand {

private:

public:
	USER(void);
	USER( std::string &line );
	~USER( void );
	void execute( std::string & line, Client & client, Server &server );
};

#endif