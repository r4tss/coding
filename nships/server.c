#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT	8888

int main()
{
	int sfd, err = 0;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	err = (sfd = socket(AF_INET, SOCK_STREAM, 0));
	if(err == -1)
		printf("Failed to create socket\n");
	else
		printf("Created socket\n");

	err = bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
	if (err == -1)
		printf("Failed to bind\n");
	else
		printf("Binded to socket\n");
	
	err = listen(sfd, 5);
	if(err == -1)
		printf("Failed to listen");
	else
		printf("Listening...\n");

	return 0;
}
