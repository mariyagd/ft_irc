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
	bool 		mode_already_set( Channel * channel, std::vector< std::string > & command, size_t & i, size_t &j, bool status );
	bool		isValidNumber( const std::string & number) const;
	void		remove_signs( std::vector< std::string > & command );
	void		remove_extras( std::vector< std::string > & command );

};

#endif