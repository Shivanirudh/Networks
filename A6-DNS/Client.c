//DNS using UDP
//Client 

#include "DNSTable.h"

int main(int argc, char **argv){
	if (argc < 2){
		perror("\nError: IP address is to be passed as argument.\n");
	}

	//Server and Client addresses
	struct sockaddr_in server_address;
	//Buffer to handle messages
	char buffer[1024];
	//Query
	Record query;

	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0){
		perror("\nError: Socket creation failed!\n");
	}
	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));


	servaddr.sin_family = AF_INET;// Uses Internet address family
	servaddr.sin_addr.s_addr = inet_addr(INADDR_ANY);//Use any of the available addresses
	servaddr.sin_port = htons(7894);//Use port 7894
	
	int len = sizeof(Record);
	while(1){

		init(&query);
		printf("\nEnter the domain name: ");scanf(" %[^\n]", query.domain);

		if (strcmp(query.domain, "END") == 0)
			break;

		sendto(sockfd, query.domain, sizeof(query.domain), MSG_CONFIRM, (struct sockaddr *)&server_address, sizeof(server_address));
		recvfrom(sockfd, &query, sizeof(Record), MSG_WAITALL, (struct sockaddr *)&server_address, &len);

		if (!query.address[0][0])
			printf("\nNo entry in DNS!\n");
		else{
			printf("\nThe IP Address is: ");
			for (int i = 0; i < ADDR_LIMIT; i++){
				if (query.address[i][0])
					printf("%s\n", query.address[i]);
			}
			printf("\n");
		}
	}

	close(sockfd);
}