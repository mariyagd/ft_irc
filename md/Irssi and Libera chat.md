Before starting your project, take a time to try irssi services. Follow these steps:

1. Install irssi ou weechat with brew 
2. [Use Libera.Chat server to connect](https://libera.chat/guides/connect#:~:text=Chat%20Via%20TLS-,Libera.,additional%20work%20may%20be%20required.)
	Here is an exemple:
	``` 
	/connect < server's IP/ domain address > <port> < optionnal: regional specific address > < optionnal: password >
	```

	Exemple connect to libera chat server:

	```
	/connect irc.libera.chat 6697 irc.eu.libera.chat 
	```

3. See the communication between the server and irssi. This will help you understand the replies that your server should send. You need to specify a file name and its placement. For exemple: ~/Desktop/server.log
 
```
/RAWLOG OPEN ~/Desktop/server.log
```

4. CAP

The LS subcommand is used to list the capabilities supported by the server. The client should send an LS subcommand with no other arguments to solicit a list of all capabilities.

If a server receives an `LS` subcommand while client registration is in progress, it MUST suspend registration until an `END` subcommand is received from the client.

When sent by the server, the last parameter is a space-separated list of capabilities (possibly with values, depending on the CAP LS Version described below). If no capabilities are available, an empty parameter MUST be sent.

Example:

```
Client: CAP LS
Server: CAP * LS :multi-prefix sasl
```
Example with no available capabilities:

```
Client: CAP LS
Server: CAP * LS :
```