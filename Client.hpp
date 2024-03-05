#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"
# include "Channel.hpp"
# include <set>

class Channel;

class Client  {

private:

	int 				_socket;
	bool				_is_registered;
	bool				_gave_password;
	int					_id;
	std::string			_nickname;
	std::string			_username;
	std::string			_hostname;
//	std::string			_servname;
	std::string			_realname;

	static int  		_id_num;
	static int 			_serverSocket;
	static std::string	_serverName;

	std::vector< Channel * > _channels;

public:
	Client( void );
	Client( int socket );
	~Client( void );




	void			setServer( const int & socket, const std::string & serverName );
	void			setConnecion( const int & socket, const struct sockaddr & addr /*, const socklen_t & addrlen*/ );
	void			setRegistered( bool );
	void			setNickname( std::string nick );
	void			setUsername( const std::string & username );
//	void			setHostname( std::string hostname );
//	void			setServname( std::string servname );
	void			setRealname( std::string realname );
	void			setGavePassword( bool status );

	bool			getGavePassword( void ) const;
	const int &		getSocket( void ) const;
	int				getNicknameId( void ) const;
	std::string		getNickname( void ) const;
	std::string		getUsername( void ) const;
	std::string		getHostname( void ) const;
	std::string		getServname( void ) const;
	std::string		getRealname( void ) const;

	// CHANNELS-------------------------------------------------------------------------------------------------------------
	void								addChannel( Channel * channel );
	void								removeChannel( Channel * channel );
	std::set< int > 					getAllClientsInAllChannels( void ) const;

	bool			isRegistered( void ) const;

	void			cleanClient( void );

	void			closeSocket( void );

	void 			printInfo( void );

	bool			operator==( const Client & rhs ) const;
	bool			operator!=( const Client & rhs ) const;
	int				operator[](int) const;
	bool			operator>=(int) const;

};

#endif