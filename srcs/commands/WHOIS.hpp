#ifndef WHOIS_HPP
# define WHOIS_HPP

# include "ACommand.hpp"

class WHOIS : public ACommand {

public:
	WHOIS(void);
	~WHOIS( void );
	void execute( std::vector< std::string > & command, Client & client, Server &server );
};

#endif