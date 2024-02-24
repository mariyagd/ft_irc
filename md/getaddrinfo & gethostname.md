
#### 1. **gethostname()**:

```
int gethostname(char *name, size_t namelen);
```

- This function is useful when you need to identify the name of the machine where your program ( your server) is running.
- It takes two arguments: a pointer to a character array where the hostname **will** be stored, and the size of that array. 
- After calling `gethostname()`, the character array will contain the hostname of the local machine. 

```
    char hostname[1024];  
    memset(hostname, 0, 1024);  
    gethostname(hostname, 1023); 
```

#### 2. **getaddrinfo()**: 
```
int getaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hints, struct addrinfo **servinfo);
```
- You obtain the `hostname` of the local machine using `gethostname()`. 
- Then, you use `getaddrinfo()` by passing the hostname as an argument. 
- This function will extract all the IP addresses available on this machine that your clients can use to connect to your server (because your server is on this machine). Instead of getting all the available addresses, you can specify which ones you are interested in. Will your server accept only IPv4 connections, IPv6, or both? Will you use TCP or UDP connections? 
- To determine whether your server will exclusively accept IPv4 connections, IPv6 connections, or both, as well as the type of network connections to use (TCP or UDP), you specify these preferences in a `struct addrinfo hints`. Based on these specifications, the `getaddrinfo()` function will retrieve all available information from your system and store it in a `struct addrinfo servinfo`. 
- By specifying the information you need, `getaddrinfo()` will filter the results and return only the information you need in a linked list `servinfo`. You can then iterate through the linked list of `addrinfo` structures to extract the desired address information for further use in your program, such as establishing network connections.
- Examples:
	- **`ai_family`**: set this field to either `AF_INET` for IPv4 addresses, `AF_INET6` for IPv6 addresses, or `AF_UNSPEC` to allow either IPv4 or IPv6 addresses.
	- **`ai_socktype`**: the type of communication you would like to use. `SOCK_STREAM` for TCP streams and `SOCK_DGRAM` for UDP streams.
	- **`ai_flags`**: with the flag `AI_PASSIVE`, you want to bind your socket to all available network interfaces of your machine.
	- **`ai_protocol`** - most of the cases you can leave it to 0. Thus, `getaddrinfo()` will automatically choose the apropriate protocol according to the socket type and the adress familly that were previously specified. `IPPROTO_TCP` for TCP and `IPPROTO_UDP` for UDP.
- **`servname`**: This is the service name or port number associated with the address. The client has to use this port to connect to your server. This parameter is optional and can be set to `nullptr` if not needed.

```
void    Server::get_addrinfo()  
{  
	std::string     _port;
	struct addrinfo _hints;
	struct addrinfo *_servinfo;
  
    _hints.ai_family = PF_INET;                     // don't care IPv4 or IPv6  
    _hints.ai_socktype = SOCK_STREAM;              // TCP stream sockets  
    _hints.ai_flags = AI_PASSIVE;                 // fill in my IP for me  
    _hints.ai_protocol = 0;                         // any protocol  
  
    int    status = getaddrinfo( hostname, std::to_string(_port).c_str(), &_hints, &_servinfo );  
    if (status != 0)  
    {  
       std::string error_msg = "Getaddrinfo error: " + std::string( gai_strerror(status) );  
       throw ServerException( error_msg.c_str() );  
    }  
  
    std::cout << " Getting address info successful" << std::endl;  
  
    std::cout << "Server is hosted on: " << hostname << std::endl;  
    
    std::cout << " To connect to this server, use this/these adresse/s: "<< std::endl;  
  
    for (struct addrinfo *ptr = _servinfo; ptr != NULL; ptr = ptr->ai_next)  
    {  
       if (ptr->ai_family == AF_INET)  
       {  
          struct sockaddr_in *ipv4 = reinterpret_cast< struct sockaddr_in * >( ptr->ai_addr );  
          char IP_adress[INET_ADDRSTRLEN];  
          inet_ntop( AF_INET, &( ipv4->sin_addr ), IP_adress, INET_ADDRSTRLEN );  
          std::cout << " IPv4: " << IP_adress << std::endl;  
       }  
       else if (ptr->ai_family == AF_INET6)   
       {  
          struct sockaddr_in6 *ipv6 = reinterpret_cast< struct sockaddr_in6 *>( ptr->ai_addr );  
          char IP_adress[INET6_ADDRSTRLEN];  
          inet_ntop( AF_INET6, &( ipv6->sin6_addr ), IP_adress, INET6_ADDRSTRLEN );  
          std::cout << " IPv6: " << IP_adress << END << std::endl;  
       }  
    }  
}
```

#### 3. socket()
```
`int sockfd = socket(AF_INET, SOCK_STREAM, 0);`
```
#### 4. bind()
