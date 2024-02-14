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
		char *			_error_message;

	public:
		ServerException(char *error_message) : _error_message(error_message) {};
		virtual const char *what() const throw() {
			return _error_message;
		};
	};
};

#endif