#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "DNS.h"

#define MAX 1024

int main(int argc, char **argv)
{

	Entry table[MAX_DOMAIN], *result;
	bzero(table, MAX_DOMAIN * sizeof(Entry));

	if (argc < 2)
	{
		fprintf(stderr, "Error: Enter port number for server as second argument!\n");
		exit(EXIT_FAILURE);
	}

	int PORT = atoi(argv[1]);
	int sockfd, new_fd, len;
	struct sockaddr_in servaddr, cliadrr;
	char buff[30];
	string domain, address;
	int n;

	// UDP socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		fprintf(stderr, "Error: Socket bind failed!\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("Socket bind successfull\n");

	len = sizeof(cliadrr);
	// Accept the data packet from client and verification

	createEntry(table, "google.com", "192.168.1.1");
	createEntry(table, "yahoo.com", "194.12.34.12");
	createEntry(table, "google.com", "17.10.23.123");

	int opt;
	while (1)
	{
		printTable(table);

		printf("Do you want to update table 1/0: ");
		scanf("%d", &opt);

		if (!opt)
			break;

		printf("Enter domain: ");
		scanf(" %[^\n]", domain);
		printf("Enter address: ");
		scanf(" %[^\n]", address);

		int rval = createEntry(table, domain, address);
		if(rval)
			printf("Successfully added entry!\n\n");
	}

	printf("DNS Server Running\n");

	while (1)
	{
		recvfrom(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr *)&cliadrr, &len);
		result = getAddress(table, buff);
		sendto(sockfd, result, sizeof(Entry), MSG_CONFIRM, (struct sockaddr *)&cliadrr, len);
	}

	close(sockfd);
}
