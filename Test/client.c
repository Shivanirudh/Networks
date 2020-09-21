#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>

#include "DNS.h"

#define MAX 1024
#define SA struct sockaddr
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Please pass port number of server as second argument!\n");
		exit(EXIT_FAILURE);
	}
	int PORT = atoi(argv[1]);

	Entry query;
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	char buff[30] = {0};

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		fprintf(stderr, "Error: Socket creation failed!\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("Socket creation successfull!\n");

	bzero(&servaddr, sizeof(servaddr));
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	int len = sizeof(Entry);
	while(1)
	{

		bzero(&query, sizeof(Entry));
		printf("Enter the domain name: ");
		scanf(" %[^\n]", query.domain);

		if (strcmp(query.domain, "END") == 0)
			break;

		sendto(sockfd, query.domain, sizeof(query.domain), MSG_CONFIRM, (struct sockaddr *)&servaddr, sizeof(servaddr));

		recvfrom(sockfd, &query, sizeof(Entry), MSG_WAITALL, (struct sockaddr *)&servaddr, &len);

		if (!query.address[0][0])
			printf("No entry in DNS!\n");
		else
		{
			printf("The IP Address is: \n");
			for (int i = 0; i < MAX_ADDR; i++)
			{
				if (query.address[i][0])
					printf("%s\n", query.address[i]);
			}
			printf("\n");
		}
	}

	close(sockfd);
}