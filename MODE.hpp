#ifndef MODE_HPP
# define MODE_HPP

# include "ACommand.hpp"

class MODE : public ACommand {
	
	public:
	MODE( void );
	~MODE( void );

	void		execute( std::vector< std::string > & command, Client & client, Server &server );
	void		setChannelMode( Client & client,  std::vector< std::string > & command, Channel * channel );
	bool		errorsInModeCommand( std::vector< std::string > & command, Client & client, Channel * channel );
};

#endif