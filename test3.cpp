#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
	struct sockaddr_in sa;
	char host[1024];

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("adresse_ip");

	if (getnameinfo((struct sockaddr *)&sa, sizeof(sa), host, sizeof(host), NULL, 0, 0) != 0) {
		printf("Erreur lors de la recherche DNS inverse.\n");
		return 1;
	}

	printf("Nom d'hôte : %s\n", host);

	return 0;
}