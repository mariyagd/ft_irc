#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class Server {

private:
	int						_port;
	char *					_password;
	int						_socket_fd;				// for socket()
	struct sockaddr_in		_server_address;		// for bind()
	struct addrinfo 		*_hints, *_servinfo;	// for getaddrinfo()

public:
	Server(int port, char *password );
	~Server( void );

	void	socket( void );
	void	launch( void );
	void	accept( void );
	void	receive( void );
	void	send( void );


	class ServerException : public std::exception {

	private:
		std::string			_error_msg1;
		char *				_error_msg2;

	public:
		ServerException(std::string error_msg1, char *error_msg2);
		virtual const char *what() const throw();
};

#endif