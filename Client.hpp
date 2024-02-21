#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Client
{
private:

	int 				_socket;
	bool				_is_registered;
	std::string			_name;
	std::string			_nick;
	std::string			_user;
	std::string			_realname;
	std::string			_hostname;
	struct sockaddr		_addr;
	socklen_t 			_addrlen;


public:
	Client( void );
	Client( int socket );

	~Client( void );

	void		setSocket( int socket );
	void		setRegistered( bool );
	void		setName( std::string name );
	void		setNick( std::string nick );
	void		setUser( std::string user );
	void		setRealname( std::string realname );
	void		setHostname( std::string hostname );

	int			getSocket( void );
	bool		isRegistered( void );
	std::string	getName( void );
	std::string	getNick( void );
	std::string	getUser( void );
	std::string	getRealname( void );
	std::string	getHostname( void );

	void		closeSocket( void );

};

#endif