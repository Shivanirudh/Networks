//DNS using UDP
//Server

#include "DNSTable.h"

int main(int argc, char **argv){
	
	Record table[DOMAIN_LIMIT], *result;
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&table[i]);
	}

	if (argc > 1){
		perror("\nError: No arguments needed for server.\n");
	}

	//Server and Client addresses
	struct sockaddr_in server_address, client_address;
	//Buffer to handle messages
	char buffer[30];

	char *domain = (char*)calloc(100, sizeof(char));
	char *address = (char*)calloc(100, sizeof(char));

	//Server socket file descriptor
	int	sockfd = socket(AF_INET, SOCK_DGRAM, 0);//domain = IPv4, type = UDP, protocol = ip
	if (sockfd < 0)
		perror("\nError: Socket creation failed!\n");
	
	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;// Uses Internet address family
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);//Use any of the available addresses
	server_address.sin_port = htons(7894);//Use port 7894

	//Bind socket to the specified port
	if ((bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)))< 0)
		perror("\nError: Socket bind failed!\n");

	int len = sizeof(client_address);
	
	//Create record in table
	createRecord(table, "google.com", "192.168.1.1");
	createRecord(table, "yahoo.com", "194.12.34.12");
	createRecord(table, "google.com", "17.10.23.123");

	//Allow modification of table
	int opt=0;
	while(1){
		DNSTable(table);

		printf("\nDo you want to update table? 1/0: ");scanf("%d", &opt);
		if(opt != 1 )
			break;

		printf("\nEnter domain: ");scanf(" %[^\n]", domain);
		printf("\nEnter address: ");scanf(" %[^\n]", address);

		int rval = createRecord(table, domain, address);
		if(rval)
			printf("\nSuccessfully added entry\n");
	}

	printf("\nDNS Server set up\n");

	while(1){

		recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&client_address, &len);
		result = getAddress(table, buffer);
		sendto(sockfd, result, sizeof(Record), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
	}
	close(sockfd);
}