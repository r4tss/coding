#define PORT	8888

int hostGame()
{
	int sfd, cfd, len, err = 0;
	struct sockaddr_in addr, cli;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	err = (sfd = socket(AF_INET, SOCK_STREAM, 0));
	if(err == -1)
		return 0;

	err = bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
	if(err == -1)
		return 0;
	
	err = listen(sfd, 2);
	if(err == -1)
		return 0;

	len = sizeof(cli);

	cfd = accept(sfd, (struct sockaddr*)&cli, &len);
	if(cfd < 0)
		return 0;

	return cfd;
}

int joinGame(char *ip)
{
	int sfd, err = 0;
	struct sockaddr_in addr;
	char buff[2048];

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(PORT);

	err = (sfd = socket(AF_INET, SOCK_STREAM, 0));
	if(err == -1)
		return 0;

	err = (connect(sfd, (struct sockaddr*)&addr, sizeof(addr)));
	if(err == -1)
		return 0;

	return sfd;
} 
