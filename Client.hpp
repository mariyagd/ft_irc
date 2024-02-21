#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Client
{
private:

	int 				_socket;
	bool				_is_registered;
	std::string			_nickname;
	std::string			_username;
	std::string			_hostname;
	std::string			_servname;
	std::string			_realname;
	struct sockaddr		_addr;
	socklen_t 			_addrlen;


public:
	Client( void );
	Client( int socket );

	~Client( void );

	void		setSocket( int socket );
	void		setRegistered( bool );
	void		setNickname( std::string nick );
	void		setUsername( std::string user );
	void		setHostname( std::string hostname );
	void		setServname( std::string servname );
	void		setRealname( std::string realname );

	int			getSocket( void );
	bool		isRegistered( void );
	std::string	getNickname( void );
	std::string	getUsername( void );
	std::string	getHostname( void );
	std::string	getServname( void );
	std::string	getRealname( void );

	void		closeSocket( void );

	void 		printInfo( void );

};

#endif