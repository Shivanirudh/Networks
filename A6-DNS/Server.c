//DNS using UDP
//Server

#include "DNSTable.h"

int main(int argc, char **argv){
	
	Record table[DOMAIN_LIMIT];
	char *addresses = (char*)calloc(ADDR_LIMIT*2, sizeof(char));
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&table[i]);
	}

	if (argc > 1){
		perror("\nError: No arguments needed for server.\n");
		exit(0);
	}

	//Server and Client addresses
	struct sockaddr_in server_address, client_address;
	//Buffer to handle messages
	char buffer[1024];

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
	char opt='n';
	do{
		DNSTable(table);

		printf("\nDo you want to update table? y/n: ");scanf(" %c", &opt);

		if(opt == 'y' || opt == 'Y'){
			printf("\nEnter domain: ");scanf(" %[^\n]", domain);
			printf("\nEnter address: ");scanf(" %[^\n]", address);

			int rval = createRecord(table, domain, address);
			if(rval)
				printf("\nSuccessfully added entry!!\n");
		}
	}while(opt == 'y'||opt == 'Y');

	printf("\nDNS Server set up\n");

	while(1){
		bzero(&buffer, sizeof(buffer));
		recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&client_address, &len);
		
		strcpy(addresses, getAddress(table, buffer));
		sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
	}
	close(sockfd);
}

/*
Output:
 ________________________________________ 
|___Domain Name___|________Address_______|
| google.com      | 192.168.1.1          |
|                 | 17.10.23.123         |
|_________________|______________________|
| yahoo.com       | 194.12.34.12         |
|_________________|______________________|


Do you want to update table? y/n: y

Enter domain: google.com

Enter address: 255.254.253.252

Successfully added entry!!
 ________________________________________ 
|___Domain Name___|________Address_______|
| google.com      | 192.168.1.1          |
|                 | 17.10.23.123         |
|                 | 255.254.253.252      |
|_________________|______________________|
| yahoo.com       | 194.12.34.12         |
|_________________|______________________|


Do you want to update table? y/n: y

Enter domain: youtube.com

Enter address: 111.234.15.1

Successfully added entry!!
 ________________________________________ 
|___Domain Name___|________Address_______|
| google.com      | 192.168.1.1          |
|                 | 17.10.23.123         |
|                 | 255.254.253.252      |
|_________________|______________________|
| yahoo.com       | 194.12.34.12         |
|_________________|______________________|
| youtube.com     | 111.234.15.1         |
|_________________|______________________|


Do you want to update table? y/n: n

DNS Server set up
*/