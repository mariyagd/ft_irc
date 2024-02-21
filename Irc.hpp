#ifndef IRC_HPP
# define IRC_HPP

# define MAX_CONNECTIONS 10
# define MSG_MAX_SIZE 512
# include "Colors.h"

# include <iostream>
# include <sys/socket.h>		// for socket, getpeername
# include <errno.h>				// for strerror
# include <stdio.h>				// for strerror, sprintf
# include <strings.h> 			// for bzero
# include <string.h>			// for strcmp
# include <netinet/in.h>		// for struct sockaddr_in
# include <sys/types.h>			// for getaddrinfo, gai_strerror
# include <netdb.h>				// for getaddrinfo, gai_strerror
# include <stdexcept>			// for std::exception
# include <unistd.h>			// for close
# include <arpa/inet.h>			// for inet_addr
# include <sys/select.h>		// for select
# include <signal.h>			// for signal				<-------
# include <csignal>				// for std::sig_atomic_t	<-------
# include <fcntl.h>
# include <vector>
# include <sstream>				// for std::stringstream
# include <iomanip>

#endif