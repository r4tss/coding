#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT	8888

void hostGame()
{
	int sfd, cfd, len, err = 0;
	struct sockaddr_in addr, cli;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	err = (sfd = socket(AF_INET, SOCK_STREAM, 0));
	if(err == -1)
		printf("Failed to create socket\n");
	else
		printf("Created socket\n");

	err = bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
	if(err == -1)
		printf("Failed to bind\n");
	else
		printf("Binded to socket\n");
	
	err = listen(sfd, 2);
	if(err == -1)
		printf("Failed to listen");
	else
		printf("Listening...\n");

	len = sizeof(cli);

	cfd = accept(sfd, (struct sockaddr*)&cli, &len);
	if(cfd < 0)
		printf("Failed to accept connection");
	else
		printf("Accepted client\n");
	
	char buff[2048];
	
	read(cfd, buff, 2048);
	printf("%s", buff);

	close(sfd);
}

void joinGame(char *ip)
{
	int sfd, err = 0;
	struct sockaddr_in addr;
	char buff[2048];

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(PORT);

	err = (sfd = socket(AF_INET, SOCK_STREAM, 0));
	if(err == -1)
		printf("Failed to create socket\n");
	else
		printf("Created socket\n");

	err = (connect(sfd, (struct sockaddr*)&addr, sizeof(addr)));
	if(err == -1)
		printf("Failed to connect to server\n");
	else
		printf("Connected to server...\n");

	send(sfd, buff, strlen(buff), 0);
	printf("Sent message!\n");

	close(sfd);
} 
