#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Commands.hpp"
# include "Channel.hpp"

class Client;
class Commands;
class Channel;

class Server {

protected:
	int								_port;
	int								_servSock;				// for socket()
	struct sockaddr_in				_server_address;		// for bind()
	struct addrinfo 				_hints;					// for getaddrinfo()
	struct addrinfo					*_servinfo;				// for getaddrinfo()

	std::string						_server_name;

	fd_set							read_fd_set;
//	fd_set							write_fd_set;
	struct sockaddr_storage			new_addr;
	socklen_t						addrlen;

	std::vector< Client >			_connections;
	std::vector<Channel *>			_channelName;
	char 							message[MSG_MAX_SIZE];

//	const char *					_password;
	std::string 					_password;
public:

	// signal handling
	static volatile std::sig_atomic_t 	_shutdown_server;
	static void		handler(int sig_code);
	void			sig_handler( void );

	Server(int port, char *password );
	Server		&operator=(const Server &rhs);
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

	void register_client( int i );


	const std::string &					getPassword( void ) const;
	std::vector< Client > &				getConnections( void );
	void								addChannel(Channel *channel);
	Channel	*							getChannel(const std::string& channelName);
	char *								getServerName( void ) const;
	std::string							getProtocolFamilyName(int family);
	void 								addClientToChannel(const std::string& channelname, Client* client);
	void								sendToChannel(std::string&kickMessage, std::string& channel);

	class ServerException : public std::exception {

	private:
		const char *				_error_msg;

	public:
		ServerException( const char *error_msg );
		virtual const char *what() const throw();
	};
};

#endif