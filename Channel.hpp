#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <iostream>
# include <string>
# include "Client.hpp"
# include "Get.hpp"
# include <sys/time.h>
# include <set>

class Client;

class Channel {

private:
	std::string					_name;
	std::vector< Client * >		_clients;
	std::vector< int >			_operators;
	std::vector< int >			_invited_list;
	long						_creation_time;
	//modes
	//topic

	int 			_limit;
	std::string		_key;
	std::string		_topic;

	bool	limitMode;
	bool	inviteMode;
	bool	topicMode;
	bool	keyMode;

	int 	_topicSetter;
	long 	_topicTime;

public:
	Channel( void );
	Channel( const std::string& channelName );
	Channel( Channel const & src) ;
	Channel & operator=( Channel const & rhs );
	~Channel( void );

	const std::string &					getChannelName( void );
	std::vector< Client * > &			getAllClients( void );
	std::set< int > 					getAllClientsSockets( void );
	const long &						getCreationTime( void ) const;
	const std::vector< std::string >	getOperatorsName( void ) const;
	const std::vector< int > &			getOperatorsID( void ) const;

	int									getOperatorSocket( int operator_id ) const;
	void 								addOperator( int id );
	void 								removeOperator( int id );

	void 								addInvited( int id );
	void 								removeInvited( int id );
//	Client * 							getInvitedClient( int id ) const;
	bool								isClientInvited( int id ) const;

	void								setLimitMode( bool mode, const int & limit );
	void								setInviteMode( bool mode );
	void								setTopicMode( bool mode );
	void								setKeyMode( bool mode, const std::string & key);

	std::string 						getCurrentChannelModes( void ) const;
	bool								getLimitMode( void ) const;
	bool								getInviteMode( void ) const;
	bool								getTopicMode( void ) const;
	bool								getKeyMode( void ) const;

	const std::string &					getKey( void ) const;
	int 								getLimit( void ) const;
	const std::string &					getTopic( void ) const;

	void								setTopic( const std::string & topic, int setter_id );
	Client *	 						getTopicSetter( void ) const;
	long 								getTopicCreationTime( void ) const;

	bool								clientIsInChannel( const Client * client ) const;
	Client *							getClientByNickname( const std::string & nickname ) const;
	int 								isClientIsOperator( int clientId )const;
	void								addClient( Client & client );
	void								removeClient( const std::string & nickname );

	bool								operator==( const Channel & rhs ) const;

	void								print_channels_info( void ) const;
	void								print_channel_modes( void ) const;


	};

# endif