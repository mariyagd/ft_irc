#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class Server {

private:
	int						_port;
	const char *			_password;
	int						_servSock;				// for socket()
	struct sockaddr_in		_server_address;		// for bind()
	struct addrinfo 		_hints, *_servinfo;	// for getaddrinfo()

	int						all_connections[MAX_CONNECTIONS];
	fd_set					read_fd_set;
	int						new_fd;
	struct sockaddr_storage	new_addr;
	socklen_t				addrlen;

public:
	Server(int port, char *password );
	~Server( void );

	void	launch( void );
	void	get_addrinfo( void );
	void	socket( void );
	void	bind( void );
	void	listen( void );
	void	select( void );
	void	accept( void );
//	void	receive( void );
//	void	send( void );

	ssize_t handle_log_record(int in_h, int out_h);

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