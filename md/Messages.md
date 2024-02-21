

- Servers and clients send each other messages which may or may not generate a reply.  If the message contains a valid command, as described in later sections, the client should expect a reply as specified but it is not advised to wait forever for the reply; client to server and server to server communication is essentially asynchronous in nature.
- Each IRC message may consist of up to three main parts: the prefix (optional), the command, and the command parameters (of which there may be up to 15).  The prefix, command, and all parameters are separated by one (or more) ASCII space character(s) (0x20).
- Empty messages are silently ignored
- CR and LF are message separators.