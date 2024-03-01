#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <iostream>
# include <string>
# include "Client.hpp"
# include "Get.hpp"
# include <sys/time.h>

class Client;

class Channel {

private:
	std::string					_name;
	std::vector< Client * >		_clients;
	std::vector< int >			_operators;
	long						_creation_time;
	//modes
	//topic
public:
	Channel( void );
	Channel( const std::string& channelName );
	Channel( Channel const & src) ;
	Channel & operator=( Channel const & rhs );
	~Channel( void );

	const std::string &					getChannelName( void );
//	Client &							getClient( const std::string & nickname );
	std::vector< Client * > &			getAllClients( void );
	const long &						getCreationTime( void ) const;
	const std::vector< std::string >	getOperatorsName( void ) const;

	int									getOperatorSocket( int operator_id ) const;
	void 								addOperator( int id );


	bool								isClientInChannel( const Client * client ) const;
	int 								isClientIsOperator(int clientId)const;
	void								addClient( Client & client );
	void								removeClient( const std::string & nickname );

};

# endif