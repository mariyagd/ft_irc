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
	Channel(void);
	Channel(const std::string& channelName);
	Channel(Channel const & src);
	Channel & operator=(Channel const & rhs);
	~Channel( void );

	const std::string &			getChannelName( void );
	std::vector< Client * > &	getAllClients( void );
	const long &				getCreationTime( void ) const;

	bool					isClientInChannel( const Client * client ) const;
	int 					isClientIsOperator(int clientId)const;
	void					addClient( Client & client );
	void					removeClient( const std::string & nickname );
	int getOperatorSocket()const;

};

# endif