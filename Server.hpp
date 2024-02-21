#ifndef SERVER_HPP
# define SERVER_HPP


# include "Client.hpp"

class Server {

private:
	int								_port;
	const char *					_password;
	int								_servSock;				// for socket()
	struct sockaddr_in				_server_address;		// for bind()
	struct addrinfo 				_hints;					// for getaddrinfo()
	struct addrinfo					*_servinfo;				// for getaddrinfo()

	fd_set							read_fd_set;
	fd_set							write_fd_set;
	struct sockaddr_storage			new_addr;
	socklen_t						addrlen;

	std::vector< Client >			_connections;


public:

	// signal handling
	static volatile std::sig_atomic_t 	_shutdown_server;
	static void		handler(int sig_code);
	void			sig_handler( void );

	Server(int port, char *password );
	~Server( void );

	void	shutdown( void );

	void	launch( void );
	void	get_addrinfo( void );
	void	socket( void );
	void	bind( void );
	void	listen( void );
	void	loop( void );


	void reset_fds( void );
	void accept( void );
	void receive( int i );

	void process( std::string &msg, int i );
	void process_registration( std::string &msg, int i );

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