//DNS using UDP
//Client 

#include "DNSTable.h"

int main(int argc, char **argv){
	printf("\nHI1\n");
	if (argc < 2){
		perror("\nError: IP address is to be passed as argument.\n");
	}
	printf("\nHI2\n");
	//Server addresses
	struct sockaddr_in server_address;
	//Buffer to handle messages
	char buffer[1024];
	//Query
	Record query;
	printf("\nHI3\n");
	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0){
		perror("\nError: Socket creation failed!\n");
	}
	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));
	printf("\nHI4\n");

	server_address.sin_family = AF_INET;// Uses Internet address family
	printf("\nHI4.1\n");
	server_address.sin_addr.s_addr = INADDR_ANY;//Use any of the available addresses
	printf("\nHI4.2\n");
	server_address.sin_port = htons(7894);//Use port 7894
	printf("\nHI5\n");
	int len = sizeof(Record);
	while(1){
		printf("\nHI6\n");
		init(&query);
		//query->domain = (char*)calloc(100, sizeof(100));
		//for(int i =0;i<ADDR_LIMIT;i++)
		//	query->address[i] = (char*)calloc(100, sizeof(100));
		printf("\nHI7\n");
		printf("\nEnter the domain name: ");scanf(" %[^\n]", query.domain);

		if (strcmp(query.domain, "END") == 0)
			break;
		printf("\n%s\n", query.domain);
		sendto(sockfd, &query, sizeof(Record), MSG_CONFIRM, (struct sockaddr *)&server_address, sizeof(server_address));
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