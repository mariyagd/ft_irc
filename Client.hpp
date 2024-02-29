#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Client  {

private:

	int 				_socket;
	bool				_is_registered;
	bool				_gave_password;
	int					_id;
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

	static int 			_serverSocket;

	std::string getnickname() const;

	void		setServer( const int & socket );
	void		setConnecion( const int & socket, const struct sockaddr & addr, const socklen_t & addrlen );
	void		setRegistered( bool );
	void		setNickname( std::string nick );
	void		setUsername( std::string user );
	void		setHostname( std::string hostname );
	void		setServname( std::string servname );
	void		setRealname( std::string realname );
	void		setGavePassword( bool status );

	bool			getGavePassword( void ) const;
	const int &		getSocket( void ) const;
	int				getNicknameId( void ) const;
	std::string		getNickname( void ) const;
	std::string		getUsername( void ) const;
	std::string		getHostname( void ) const;
	std::string		getServname( void ) const;
	std::string		getRealname( void ) const;

	bool			isRegistered( void ) const;


	void		cleanClient( void );

	void		closeSocket( void );

	void 		printInfo( void );

	bool		operator==(const Client & rhs) const;

};

#endif