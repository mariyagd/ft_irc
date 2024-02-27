#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <iostream>
# include <string>
# include "Client.hpp"
# include "Get.hpp"

class Client;

class Channel {

private:
	std::string					_name;
	std::vector< Client * >		_clients;
	std::vector< int >			_operators;
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
//	Client &					getClient( const std::string & nickname );
//	std::vector<int>			getOperator( int id );
//
//	void					setName( std::string name );
//
	bool					isClientInChannel( const Client * client ) const;
//	bool					isClientInChannel( const std::string & nickname ) const;
//	bool					isOperator( const std::string& nickname );
//	bool 					isClientIsOperator( const std::string& nickname );

	void					addClient( Client & client );
	void					removeClient( const std::string & nickname );

};

# endif