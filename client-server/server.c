#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main (int arc, char ** argv)
{
	if (arc != 2)
	{
		printf("incorrect args\n");
		return 1;
	}
	char port[10];
	strcpy(port,argv[1]);
	int sock;
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket error\n");
		return 1;
	}
	struct sockaddr_in saddr;//server addres
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(atoi(port));
	if( bind( sock, (sockaddr*)&saddr, sizeof(saddr)) == -1 )
	{
		printf("bind error\n");
		return 1;
	}
	if( listen( sock , 10 ) == -1 )
	{
		printf("listen error\n");
		return 1;
	}
	struct sockaddr_in caddr;//client addres
	while (1)
	{
		int	csock, sizesockaddr = sizeof(saddr);
		if ( (csock = accept(sock, (struct sockaddr*)&caddr, (socklen_t*)&sizesockaddr) ) == -1 )
		{
			printf("accept error\n");
			return 1;
		}
		else
		{
			char temp[512];
			read(csock, &temp, 512);
			int i,length = strlen(temp);
			char result[512];
			for (i = 0; i < length; ++i)
			{
				result[i] = temp[length - i - 1];
				if (i == (length - 1) )
					result[i+1] = '\0';
			}
			write(csock, &result, 512);
			close(csock);
		}
	}
	return 0;
}
