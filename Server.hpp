#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"
# include "Client.hpp"
# include "Commands.hpp"
# include "ChannelMenager.hpp"

class Client;
class Commands;
//class ChannelMenager;

class Server : public ChannelMenager {

private:
	int									_port;
	std::string 						_password;
	fd_set								_read_fd_set;
	std::vector< Client >				_connections;
	static volatile std::sig_atomic_t 	_shutdown_server;

public:
	// signal handling
	static void		handler(int sig_code);
	void			sig_handler( void );

	Server(int port, char *password );
	~Server( void );

	void	shutdown( void );

	void	launch( void );
	void	socket( void );
	void	socket_options( void );
	void	bind( void );
	void	listen( void );
	void	loop( void );

	void reset_fds( void );
	void accept( void );
	void receive( int i );

	void register_client( int i );

	const std::string &					getPassword( void ) const;
	std::vector< Client > &				getConnections( void );
	std::string							getProtocolFamilyName(int family);
	void	sendToChannel(std::string&message, std::string& channel);

//	ChannelMenager &						getChanneltMenager( void );


	class ServerException : public std::exception {

	private:
		const char *				_error_msg;

	public:
		ServerException( const char *error_msg );
		virtual const char *what() const throw();
	};
};

#endif