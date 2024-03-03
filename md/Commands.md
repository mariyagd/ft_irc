- The server to which a client is connected is required to parse the complete message, and return any appropriate errors.
- If multiple parameters is presented, then each MUST be checked for validity and appropriate responses MUST be sent back to the client.

#### Registration
- The RECOMMENDED order for a client to register is as follows:
	1. PASS message
	2. NICK message 
	3. USER message
	4. Service message

- A "PASS" command is not required for a client connection to be registered, but it MUST precede the latter of the NICK/USER combination (for a user connection)
- NICK is the nickname that you will be visible in IRC network. If you don't specify it while connecting, irssi will automatically use your username as nick.
- USER message consist of:
	- username -> wich will be the name of your machine
	- hostname -> the name of your host machine ( your computer )
	- servname -> the server you connect to
	- realname -> your real name ( based on your machine )
- Upon success, the client will receive an RPL_WELCOME
- If the selected nickname during registration is already in use, the server must iteratively modify it until an available one is found and must **send error message to the client `ERR_NICKNAMEINUSE`**.  The procedure is illustrated using the example nickname `lola`:

	- Initially, an underscore is appended to the original nickname, resulting in `lola_`.
	- If this modified nickname is still taken, sequential numbers are added starting from 1. For instance: `lola1`, `lola2`, and so forth, until an available nickname is obtained, such as `lola10`, `lola11`, and beyond.
##### Examples of registration

1. Specifying only the server abd the port, irssi will get you informations from your computer. 
```
/CONNECT <server address/name> <port>
```
##### Replies upon registration:
The server sends Replies 001 to 004 to a user upon successful registration.

###### 001    RPL_WELCOME 
- indicates that the connection is now registered and known the to the entire IRC network. The reply message MUST contain the full client identifier upon which it was registered.

```
":<serverName> 001 Welcome to the Internet Relay Network <nick>!<user>@<host>"
```
###### 002    RPL_YOURHOST
```
:<serverName> 002 Your host is <servername>, running version <ver>
```
###### 003    RPL_CREATED

```
:<serverName> 003 This server was created <date>
```
###### 004    RPL_MYINFO
```
:<serverName> 004 <servername> <version> <available user modes> <available channel modes>
```
#### PASS
- A "PASS" command is not required for a client connection to be registered, but it MUST precede the latter of the NICK/USER combination (for a user connection)
- Currently this requires that user send a PASS command before sending the NICK/USER combination.
##### Replies:

###### 461 ERR_NEEDMOREPARAMS
```
:<serverName> 461 <command> :Not enough parameters
```
462 ERR_ALREADYREGISTRED
#### NICK
- NICK command is used to give user a nickname or change the existing one.
##### Replies:

###### 433    ERR_NICKNAMEINUSE
- Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname.
```
:<serverName> 433 <nick> :Nickname is already in use
```
###### 431    ERR_NONICKNAMEGIVEN
-  Returned when a nickname parameter expected for a command and isn’t found.
```
:<serverName> 434 :No nickname given
```
ERR_UNAVAILRESOURCE
ERR_ERRONEUSNICKNAME 
ERR_NICKCOLLISION 
ERR_RESTRICTED

###### Successful change:
```
:<oldNick>!<user>@<host> NICK <newNick>
```
#### USER
- The `USER` command is used at the beginning of a connection to specify the username and realname of a new user.
- `<realname>` must be the last parameter because it may contain SPACE `(' ')` characters, and should be prefixed with a colon (`:`) if required.
- Servers MAY use the [Ident Protocol](http://tools.ietf.org/html/rfc1413) to look up the ‘real username’ of clients. If username lookups are enabled and a client does not have an Identity Server enabled, **the username provided by the client SHOULD be prefixed by a tilde `'~'` to show that this value is user-set.**
- to change your nick and username upon registration:
```
/set nick votrenom
/set user_name votrenom

/save

reconnect
```
#### OPER
`/OPER <name> <password>`
- A normal user uses the OPER command to obtain operator privileges. The combination of `<name>` and `<password>` are REQUIRED to gain Operator privileges. Upon success, the user will receive a MODE message (see section 3.1.5) indicating the new user modes.
##### Replies:
ERR_NEEDMOREPARAMS
ERR_NOOPERHOST
ERR_PASSWDMISMATCH

RPL_YOUREOPER
#### MODE

`/MODE  ("+" or "-")  i`
- use only while being in the chat
##### Replies:
ERR_NEEDMOREPARAMS
ERR_UMODEUNKNOWNFLAG
ERR_USERSDONTMATCH


#### JOIN
- ` /JOIN <channel>` -> enter a channel
- ` /JOIN <channel>,<channel2>,<channel#N>` -> enter several channels at once
- 
- `JOIN 0` -> leave all all currently joined channels.
- Servers MUST be able to parse arguments in the form of a list of target, but SHOULD NOT use lists when sending JOIN messages to clients.
- If a JOIN is successful, the user receives a JOIN message as confirmation and is then sent the channel’s topic (using **`RPL_TOPIC`**) and the list of users who are on the channel (using **`RPL_NAMREPLY`**), which MUST include the user joining.

- Note that this message accepts a special argument ("0"), which is  a special request to leave all channels the user is currently a member of. The server will process this message as if the user had sent  a PART command (See Section 3.2.2) for each channel he is a member  of.
##### Replies:
ERR_NEEDMOREPARAMS 
ERR_INVITEONLYCHAN 
ERR_CHANNELISFULL 
ERR_NOSUCHCHANNEL 
ERR_TOOMANYTARGETS 
ERR_BANNEDFROMCHAN 
ERR_BADCHANNELKEY 
ERR_BADCHANMASK 
ERR_TOOMANYCHANNELS 
ERR_UNAVAILRESOURCE

RPL_TOPIC




#### TOPIC
  `TOPIC <channel> [ <topic> ]`
   - The topic for channel `<channel>` is returned if there is no `<topic>` given.
   - If the `<topic>` parameter is present, the topic for that channel will be changed, if this action is allowed for the user requesting it.
##### Replies:

ERR_NEEDMOREPARAMS 
ERR_CHANOPRIVSNEEDED
ERR_NOTONCHANNEL 
ERR_NOCHANMODES

RPL_NOTOPIC 
RPL_TOPIC 

Examples:
 User Wiz setting the topic.
`:WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic` 

#### INVITE
 `INVITE <nickname> <channel>`
- There is no requirement that the channel the target user is being invited to must exist or be a valid channel.
- When the channel has invite-only flag set, only channel operators may issue INVITE command.
- Only the user inviting and the user being invited will receive notification of the invitation. Other channel members are not notified.
##### Replies:
ERR_NEEDMOREPARAMS 
ERR_NOTONCHANNEL 
ERR_CHANOPRIVSNEEDED
ERR_NOSUCHNICK 
ERR_USERONCHANNEL

RPL_INVITING
RPL_AWAY

Exemple:
- Message to WiZ when he has been invited by user Angel to channel `#Dust`
	`:Angel!wings@irc.org INVITE Wiz #Dust`

#### KICK
`KICK <user>
`KICK <user> [:]<comment>`
`KICK <channel> <user> [:][<comment>]`
`KICK <channel> *( "," <channel> ) <user> *( "," <user> )[<comment>]`
- It causes the `<user>` to PART from the `<channel>` by force.
- For the message to be syntactically correct, there MUST be either one channel parameter and multiple user parameter, or as many channel parameters as there are user parameters.
##### Replies:
ERR_NEEDMOREPARAMS 
ERR_BADCHANMASK 
ERR_USERNOTINCHANNEL
ERR_NOSUCHCHANNEL 
ERR_CHANOPRIVSNEEDED 
ERR_NOTONCHANNEL

Example:
KICK message on channel `#Finnish` from WiZ to remove John from channel
`:WiZ!jto@tolsun.oulu.fi KICK #Finnish John`

#### PING
 ` PING <server1> [ <server2> ]`
- The PING command is used to test the presence of an active client or server at the other end of the connection. 
- **Servers send a PING message at regular intervals if no other activity detected coming from a connection. If a connection fails to respond to a PING message within a set amount of time, that connection is closed. **
- A PING message MAY be sent even if the connection is active.
- When a PING message is received, the appropriate PONG message MUST be sent as reply to `<server1>` (server which sent the PING message out) as soon as possible.
##### Replies: 
###### 409    ERR_NOORIGIN
- PING or PONG message missing the originator parameter.
```
:<serverName> 409 :No origin specified 
```
###### 402    ERR_NOSUCHSERVER
- PING or PONG message missing the originator parameter.
```
:<serverName> 402 <server name> :No such server
```
#### PONG
`PONG <server>
- PONG message is a reply to PING message.
- The `<server>` parameter is the name of the entity who has responded to PING message and generated this message.
##### Replies: 
###### 409    ERR_NOORIGIN
- PING or PONG message missing the originator parameter.
```
:<serverName> 409 :No origin specified
```
###### 402    ERR_NOSUCHSERVER
- Used to indicate the server name given currently does not exist.
```
:<serverName> 402 <server name> :No such server
```

#### PRIVMSG
`PRIVMSG <msgtarget> <text to be sent>`
- `PRIVMSG` is used to send private messages between users, as well as to send messages to channels.



- /part  -> exits the current channel
- /part channel -> exit a channel
- /window close ->exit a private message


### Commands for nc
- QUIT
- PART
- PING