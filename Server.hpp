#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class Server {

private:
	int						_port;
	const char *					_password;
	int						_socket_fd;				// for socket()
	struct sockaddr_in		_server_address;		// for bind()
	struct addrinfo 		_hints, *_servinfo;	// for getaddrinfo()

public:
	Server(int port, char *password );
	~Server( void );

	void	launch( void );
	void	get_addrinfo( void );
	void	socket( void );
	void	bind( void );
	void	listen( void );
	void	accept( void );
//	void	receive( void );
//	void	send( void );


	std::string		getProtocolFamilyName(int family);
	class ServerException : public std::exception {

	private:
		const char *				_error_msg;

	public:
		ServerException( const char *error_msg );
		virtual const char *what() const throw();
	};
};

#endif