//DNS using UDP
//Server

#include "DNSTable.h"

int main(int argc, char **argv){

	if (argc > 1){
		perror("\nError: No arguments needed for server.\n");
		exit(0);
	}
	//IP address(es) retrieved
	char *addresses = (char*)calloc(ADDR_LIMIT*2, sizeof(char));
	//Table at authoritative level
	Record auth_table[DOMAIN_LIMIT];
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&auth_table[i]);
	}
	//Table at top level
	Record top_table[DOMAIN_LIMIT];
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&top_table[i]);
	}
	//Table at root level
	Record root_table[DOMAIN_LIMIT];
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&root_table[i]);
	}
	//Table at local DNS server
	Record local_table[DOMAIN_LIMIT];
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&local_table[i]);
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
	createRecord(auth_table, "google.com", "192.168.1.1");
	createRecord(auth_table, "yahoo.com", "194.12.34.12");
	createRecord(auth_table, "google.com", "17.10.23.123");

	//Allow modification of table
	char opt='n';
	do{
		DNSTable(auth_table);

		printf("\nDo you want to update table? y/n: ");scanf(" %c", &opt);

		if(opt == 'y' || opt == 'Y'){
			printf("\nEnter domain: ");scanf(" %[^\n]", domain);
			printf("\nEnter address: ");scanf(" %[^\n]", address);

			int rval = createRecord(auth_table, domain, address);
			if(rval)
				printf("\nSuccessfully added entry!!\n");
		}
	}while(opt == 'y'||opt == 'Y');

	printf("\nDNS Server set up\n");

	while(1){
		printf("\n%50s\n","-");
		bzero(&buffer, sizeof(buffer));
		recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&client_address, &len);

		strcpy(addresses, getAddress(local_table, buffer));
		
		char *copy = (char*)calloc(ADDR_LIMIT*20, sizeof(char));
		strcpy(copy, addresses);
		
		printf("\nChecking local DNS server...");
		char* split = strtok(copy, " ");
	    if(split){
	    	printf("Available in local DNS server. \n");
	    	sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
		}
		else{
			printf("\nNot found in local DNS server.\n");
			
			printf("\nChecking root DNS server...");
			strcpy(addresses, getAddress(root_table, buffer));
		
			copy = (char*)calloc(ADDR_LIMIT*20, sizeof(char));
			strcpy(copy, addresses);

			split = strtok(copy, " ");
			if(split){
				printf("Available in root DNS server. \n");
				while(split){
					int val = createRecord(local_table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in local DNS server.\n");
					split = strtok(NULL, " ");
				}
	    		sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
				continue;
			}
			printf("\nNot found in root DNS server. \n");
			
			printf("\nChecking top level DNS server...");
			strcpy(addresses, getAddress(top_table, buffer));
		
			copy = (char*)calloc(ADDR_LIMIT*20, sizeof(char));
			strcpy(copy, addresses);

			split = strtok(copy, " ");
			if(split){
				printf("Available in root DNS server. \n");
				while(split){
					int val = createRecord(local_table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in local DNS server.\n");
					val = createRecord(root_table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in root DNS server.\n");
					split = strtok(NULL, " ");
				}
	    		sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
				continue;
			}
			printf("\nNot found in top level DNS server. \n");

			printf("\nChecking authoritative DNS server...");
			strcpy(addresses, getAddress(auth_table, buffer));

			strcpy(copy, addresses);
			split = strtok(copy, " ");
			if(split){
				while(split){
					int val = createRecord(local_table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in local DNS server.\n");
					val = createRecord(root_table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in root DNS server.\n");
					val = createRecord(top_table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in top DNS server.\n");
					split = strtok(NULL, " ");
				}
				sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
			}
			else{
				sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
			}
		}
		
		
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

Enter domain: youtube.com

Enter address: 255.254.253.252

Successfully added entry!!
 ________________________________________ 
|___Domain Name___|________Address_______|
| google.com      | 192.168.1.1          |
|                 | 17.10.23.123         |
|_________________|______________________|
| yahoo.com       | 194.12.34.12         |
|_________________|______________________|
| youtube.com     | 255.254.253.252      |
|_________________|______________________|


Do you want to update table? y/n: n

DNS Server set up

--------------------------------------------------

Checking local DNS server...
Not found in local DNS server.

Checking root DNS server...
Not found in root DNS server. 

Checking top level DNS server...
Not found in top level DNS server. 

Checking authoritative DNS server...
Successfully added entry in local DNS server.

--------------------------------------------------

Checking local DNS server...
Not found in local DNS server.

Checking root DNS server...
Not found in root DNS server. 

Checking top level DNS server...
Not found in top level DNS server. 

Checking authoritative DNS server...
Successfully added entry in local DNS server.

--------------------------------------------------

Checking local DNS server...
Not found in local DNS server.

Checking root DNS server...
Not found in root DNS server. 

Checking top level DNS server...
Not found in top level DNS server. 

Checking authoritative DNS server...
Successfully added entry in local DNS server.

--------------------------------------------------

Checking local DNS server...Available in local DNS server. 

--------------------------------------------------

Checking local DNS server...Available in local DNS server. 

--------------------------------------------------

Checking local DNS server...Available in local DNS server.
*/