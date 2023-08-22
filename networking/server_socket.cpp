#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#define PORT 30000
#define BACKLOG 10

int	main(int argc, char **argv)
{
	(void) argv;
	(void) argc;

	int					sockfd;;
	int					new_fd;
	struct sockaddr_in	my_addr;
	struct sockaddr_in	their_addr;
	socklen_t			sin_size;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT); //htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("IP");
	bzero(&(my_addr.sin_zero), 8);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind() failed");; // check valeur de retour, -1 si erreur, check if port is < 1024
		exit(-1);
	}
	printf("bind: %d\n", sockfd);
	
	if (listen(sockfd, BACKLOG) < 0)
	{
		perror("listen() failed");
		exit(-1);
	}

	sin_size = sizeof(struct sockaddr_in);
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

	char	*msg = "J'étais là";
	int		len;
	int		bytes_sent;

	len = strlen(msg);
	bytes_sent = send(sockfd, msg, len, 0);
	if (bytes_sent < len)
	{
		perror("send() sent less data than expected");
		exit (1);
	}
	close(sockfd);
}
