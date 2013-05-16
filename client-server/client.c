#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main (int arc, char **argv)
{
	char port[10],ip[16],message[512];
	strcpy(ip,argv[1]);
	strcpy(port,argv[2]);
	strcpy(message,argv[3]);
	if (arc != 4)
	{
		printf("incorrect args\n");
		return 1;
	}
	int sock;
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket error\n");
		return 1;
	}
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(port));
	struct in_addr inaddr;
	inet_aton(ip , &inaddr);
	saddr.sin_addr = inaddr;
	if ( connect( sock, (sockaddr*) &saddr , sizeof(saddr)) == -1 )
	{
		printf("connect error\n");
		return 1;
	}
	write(sock, &message, strlen(message));
	char result[512];
	read(sock, &result, 512);
	printf("reversed message: \"%s\"\n", result);
	return 0;
}
