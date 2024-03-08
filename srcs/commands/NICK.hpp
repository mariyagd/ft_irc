#ifndef NICK_HPP
# define NICK_HPP

# include "ACommand.hpp"

class NICK : public ACommand {

public:
	NICK(void);
	~NICK( void );

	void	execute( std::vector< std::string > & command, Client & client, Server &server );
	bool	is_unique_nickname(  std::string & nickname, int clientSocket, std::vector< Client > & connections );
};
#endif
