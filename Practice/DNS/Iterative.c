//DNS using UDP
//Server

#include "Local.h"

int main(int argc, char **argv){

	if (argc > 1){
		perror("\nError: No arguments needed for server.\n");
		exit(0);
	}
	//IP address(es) retrieved
	char *addresses = (char*)calloc(ADDR_LIMIT*2, sizeof(char));
	//Table at authoritative level
	Auth auth_table;
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&auth_table.table[i]);
	}
	//Table at top level
	Top_level top_table;
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&top_table.table[i]);
	}
	//Table at root level
	Root root_table;
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&root_table.table[i]);
	}
	//Table at local DNS server
	Local local_table;
	for(int i =0; i<DOMAIN_LIMIT;i++){
		init(&local_table.table[i]);
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
	createRecord(auth_table.table, "google.com", "192.168.1.1");
	createRecord(auth_table.table, "yahoo.com", "194.12.34.12");
	createRecord(auth_table.table, "google.com", "17.10.23.123");

	//Allow modification of table
	char opt='n';
	do{
		DNSTable(auth_table.table);

		printf("\nDo you want to update table? y/n: ");scanf(" %c", &opt);

		if(opt == 'y' || opt == 'Y'){
			printf("\nEnter domain: ");scanf(" %[^\n]", domain);
			printf("\nEnter address: ");scanf(" %[^\n]", address);

			int rval = createRecord(auth_table.table, domain, address);
			if(rval)
				printf("\nSuccessfully added entry!!\n");
		}
	}while(opt == 'y'||opt == 'Y');

	printf("\nDNS Server set up\n");

	while(1){
		printf("\n%50s\n","-");
		bzero(&buffer, sizeof(buffer));
		recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&client_address, &len);

		strcpy(addresses, getAddress(local_table.table, buffer));
		
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
			strcpy(addresses, getAddress(root_table.table, buffer));
		
			copy = (char*)calloc(ADDR_LIMIT*20, sizeof(char));
			strcpy(copy, addresses);

			split = strtok(copy, " ");
			if(split){
				printf("Available in root DNS server. \n");
				while(split){
					int val = createRecord(local_table.table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in local DNS server.\n");
					split = strtok(NULL, " ");
				}
	    		sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
				continue;
			}
			printf("\nNot found in root DNS server. \n");
			
			printf("\nChecking top level DNS server...");
			strcpy(addresses, getAddress(top_table.table, buffer));
		
			copy = (char*)calloc(ADDR_LIMIT*20, sizeof(char));
			strcpy(copy, addresses);

			split = strtok(copy, " ");
			if(split){
				printf("Available in top level DNS server. \n");
				while(split){
					int val = createRecord(local_table.table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in local DNS server.\n");
					split = strtok(NULL, " ");
				}
	    		sendto(sockfd, addresses, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
				continue;
			}
			printf("\nNot found in top level DNS server. \n");

			printf("\nChecking authoritative DNS server...");
			strcpy(addresses, getAddress(auth_table.table, buffer));

			strcpy(copy, addresses);
			split = strtok(copy, " ");
			if(split){
				while(split){
					int val = createRecord(local_table.table, buffer, split);
					if(val)
						printf("\nSuccessfully added entry in local DNS server.\n");
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