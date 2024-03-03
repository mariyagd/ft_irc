#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
	struct addrinfo hints, *addrs;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	int rval = getaddrinfo("whatismyip.akamai.com", "80", &hints, &addrs);
	if (rval != 0)
	{
		fprintf(stderr,"getaddrinfo failed: %s\n", gai_strerror(rval));
		return 1;
	}
	int s = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
	if (s == -1)
	{
		perror("socket failed");
		return 1;
	}
	rval = connect(s, addrs->ai_addr, addrs->ai_addrlen);
	if (rval == -1)
	{
		perror("connect failed");
		return 1;
	}
	char cmd[] = "GET / HTTP/1.1\nHost: whatismyip.akamai.com\n\n";
	rval = send(s, cmd, strlen(cmd), 0);
	if (rval == -1) {
		perror("send failed");
		return 1;
	}
	char buf[1000] = {0};
	rval = recv(s, buf, sizeof(buf), 0);
	if (rval == -1) {
		perror("recv failed");
		return 1;
	}
	printf("response: %s\n", buf);
	char *start = buf, *end;
	end = strchr(start, '\n');
	if (!strncmp(start, "HTTP/1.1 200 OK", end - start - 1)) {
		while (!(end[1] == '\r' && end[2] == '\n')) {
			start = end + 2;
			end = strchr(start, '\n');
		}
		start = end + 3;
		end = strchr(start, '\n');
		if (end) *end = 0;
		printf("my IP: %s\n", start);
	} else {
		printf("request failed\n");
	}
	close(s);
	freeaddrinfo(addrs);
	return 0;
}