#ifndef NICK_HPP
# define NICK_HPP

# include "ACommand.hpp"
# include "Server.hpp"
# include "Client.hpp"



class NICK : public ACommand {

private:

public:
	NICK(void);
	NICK( std::string &line );
	~NICK( void );
	void execute( std::string & line, Client & client, Server &server );
	bool		is_unique_nickname(  std::string & nickname, int clientSocket, std::vector< Client > & connections );
};
#endif
