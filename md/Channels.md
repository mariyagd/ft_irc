- A channel is a named group of one or more users which will all receive messages addressed to that channel. 
- A channel is characterized by its name, properties and current members.
- The rules governing how channels are managed are enforced by the servers.
- **A user can only become member of a channel known by the server to which the user is directly connected.**
- **When a channel existence is known to a server, the server MUST keep track of the channel members, as well as the channel modes**
- Channels names are strings (beginning with a **’&’, ’#’, ’+’ or ’!’** character, hereafter called **"channel prefix"**) of **length up to fifty (50) characters.**
- **Channel names are case insensitive.**
- Channel name is that it SHALL NOT contain any spaces (’ ’), a control G (^G or ASCII 7), a comma (’,’ which is used as a list item separator by the protocol). Also, a colon (’:’) is used as a delimiter for the channel mask.
- Channels with ’&’ as prefix are local to the server where they are created.
- Other channels are known to one (1) or more servers that are connected to the network, depending on the channel mask.
- Each channel has its own properties, which are defined by channel modes. Channel modes can be manipulated by the channel members. The modes affect the way servers manage the channels.
- The user creating a channel automatically becomes channel operator
- To ensure the uniqueness of the channel names, the channel identifier created by the server MUST follow specific rules.
- This mechanism is commonly known as "Channel Delay" and generally only applies to channels which names is prefixed with the character ’#’. When a network split occurs, servers SHOULD keep track of which channels lost a "channel operator" as the result of the break. These channels are then in a special state which lasts for a certain period of time. In this particular state, the channels cannot cease to exist. If all the channel members leave the channel, the channel becomes unavailable: the server local clients cannot join the channel as long as it is empty.

#### Privileged Channel Members:
 - INVITE
- KICK
- MODE
- PRIVMSG
- TOPIC

#### Channel Operators
- The channel operators (also referred to as a **"chop"** or **"chanop"**) on a given channel are considered to ’own’ that channel. Ownership of a channel is shared among channel operators.
- **Channel operators** are identified by the **’@’** symbol next to their nickname whenever it is associated with a channel (i.e., replies to the NAMES, WHO and WHOIS commands)

#### Channel Modes
All these modes can be manipulated by "channel operators" by using the MODE command:

- **i** - toggle the invite-only channel flag;
	- When the channel flag ’i’ is set, new members are only accepted if their mask matches Invite-list or they have been invited by a channel operator. 
	- **This flag also restricts the usage of the INVITE command to channel operators.** 
	
```
/MODE +/-i                      -> if you are in the channel window
/MODE #channel +/-i             -> if you are otside of the channel window

if the flag +i is set:
/invite nickname                 -> if you are in the channel window
/invite nickname #channel        -> if you are otside of the channel window

- only opertors can invite other users
- if a user was invited, joins and exit the channel, he needs to be invited again to rejoin the channel.
- if the channel if full, but an operator invites you, you CAN enter in the channel
```

- **t** - toggle the topic settable by channel operator only flag;
	- The channel flag ’t’ is used to restrict the usage of the TOPIC command to channel operators.
	
```
/MODE +/-t                    -> if you are in the channel
/MODE #channel +/-t           -> if you are outside of the channel

if -t is set, other clients can change the topic
operators can always change the topic
```

- **k** - set/remove the channel key (password);
	- When a channel key is set (by using the mode ’k’), servers MUST reject their local users request to join the channel unless this key is given.
	- The channel key MUST only be made visible to the channel members in the reply sent by the server to a MODE query.
	
```
/JOIN #channel
/MODE +k password.            -> if you are in the channel
/MODE #channel +k password    -> if you are otside of the channel

then other clients can connect with:
/JOIN #channel password
```

- **o** - give/take channel operator privilege;
```
/MODE +o nickname             -> the user becomes an operator (becomes @nickname)

/MODE -o nickanme             -> the user looses his operator priviliges

You can do -o to yourself or the operators to whom YOU gave priviliges
You can't do -o to operators who gave you priviliges
```

- **l** - set/remove the user limit to channel;
```
/MODE +l max_participants               -> fix a limit of participants
/MODE #channel +l max_participants      

/MODE -l                        -> unfix the limit
/MODE #channel -l
```

-  When the limit is reached, servers MUST forbid their local users to join  the channel (channel is full error is sent);
- The number specified can be inferior than the current number of channel participants 
- The value of the limit MUST only be made available to the channel members in the reply sent by the server to a MODE query.

#### Channel Identifier

The channel identifier is a function of the time. The current time (as defined under UNIX by the number of seconds elapsed since 00:00:00 GMT, January 1, 1970) is converted in a string of five (5) characters using the following base: "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890" (each character has a decimal value starting from 0 for ’A’ to 35 for ’0’).

The channel identifier therefore has a periodicity of 36^5 seconds (about 700 days).