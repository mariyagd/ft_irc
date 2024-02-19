The Socket API was developed in BSD UNIX to provide an application-level interface to the TCP/IP protocol suite. It's now the de facto standard for programming interprocess communication over TCP/IP.
Applications can use the C functions in the Socket API to create and manage local endpoints of communication, which are called sockets. Each socket is accessed via a handle, which is also referred to as a descriptor in the UNIX literature.
Each socket can be bound to a local and a remote address. These addresses define the association between two or more peers that communicate via the socket.
The Socket API contains system functions that can be classified into the following five categories:

#### 1. Local context management.
 
| Function | Description |
| ---- | ---- |
| `socket ( )` | A factory function that allocates a socket handle and returns it to the caller. |
| `bind()` | Associates a socket handle with a local or remote address. |
| `getsockname ( )` | Returns the local address to which a socket is bound. |
| `getpeername ( )` | Returns the remote address to which a socket is bound. |
| `close ( )` | Deallocates a socket handle, making it available for reuse |
#### 2. Connection establishment and connection termination.

| Function | Description |
| ---- | ---- |
| `connect ()` | Establishes a connection **actively** on a socket handle. |
| `listen()` | Indicates its willingness to listen **passively** for incoming client connection requests. |
| `accept ()` | A factory function that creates a new communication endpoint to service client requests. |
| `shutdown ( )` | Selectively terminates the read-side and/or write-side stream of a bidirectional connection |

#### 3. Data transfer mechanisms.
| Function | Description |
| ---- | ---- |
| `send()`<br>`recv()`<br> | Transmit and receive **buffers** of data via a particular I/O handle. |
| `sendto()`<br>`recvfrom()` | Exchanges connectionless datagrams, where each sendto (! call provides the networking address of the recipient. |
UNIX platforms also provide the following data transfer mechanisms:

| Function | Description |
| ---- | ---- |
| `read ( )`<br>`write ( )` | Receive and transmit **buffers** of data via a particular handle. |
| `readv ( ) `<br>`writev ( )` |  |
| `sendmsg ( )` <br>`recvmsg ( )` |  |

#### 4. Options management.

These functions that allow programmers to alter default socket behavior to enable multicasting, broadcasting, and modifying/querying the size of transport buffers:

| Function | Description |
| ---- | ---- |
| `setsockopt()` | Modifies options in different protocol stack layers. |
| `getsockopt()` | Queries options in different protocol stack layers. |
#### 5. Network addressing

Networked applications often use functions to resolve humanly readable names, such as tango. ece . uci . edu, to low-level network addresses, such as 128.195.174.35:

| Function | Description |
| ---- | ---- |
| `gethostbyname ( )`<br>`gethostbyaddr ( )` | Handle network address mapping between hostnames and IPv4 addresses. |
| `getipnodebyname ( ) `<br>`getipnodebyaddr ( )` | Handle network address mapping between hostnames and IPv4/IPv6 addresses |
| `getservbyname ( )` | Identifies services by their humanly readable names. |
A communication domain is defined by a protocol family and an address family, as follows:

#### 1. Protocol family.
Today's networking environments include a large number of protocols that can offer a variety of communication services, such as connection-oriented reliable delivery, unreliable multicast, etc. **A protocol family is a collection of protocols that offers a distinct set of related services.** When creating a socket using the Socket API, the protocol is specified by a combination of the following two parameters: 
- **Protocol family** - for example, UNIX-domain (`PF_UNIX`), Internet-domain IPv4 (`PF_INET`) and IPv6 (`PF_INET6`), ATM (`PF_ATMSVC`), X.25 (`PF_x25`), Appletalk (`PF_APPLETALK`), and so on. 
- **Service type** - for example, sequenced, reliable bytestream (`SOCK_STREAM`), unreliable datagram (`SOCK_DGRAM`), and so on. 
	For example, **the TCP/IP protocol is specified by passing the `PF_INET` (or `PF_iNET6`) and `SOCK_STREAM` flags to the `socket()` function.**
- **Address family** -  An address family defines an address format that characterizes the size of an address in bytes, as well as the number, type, and order of its fields. In addition, an address family defines a set of functions that interpret the address format, for example, to determine the subnet where an IP datagram is destined. Address families correspond closely to protocol families, for example, the IPv4 address family `AF_INET` works only with the IPv4 protocol family `PF_INET`.