//DNS using UDP
//Client 

#include "DNSTable.h"

int main(int argc, char **argv){

	if (argc < 2){
		perror("\nError: IP address is to be passed as argument.\n");
		exit(0);
	}

	//Server addresses
	struct sockaddr_in server_address;
	//Buffer to handle messages
	char buffer[1024];
	//Query
	Record *query = (Record*)malloc(sizeof(Record));

	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0){
		perror("\nError: Socket creation failed!\n");
	}
	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;// Uses Internet address family
	server_address.sin_addr.s_addr = INADDR_ANY;//Use any of the available addresses
	server_address.sin_port = htons(7894);//Use port 7894

	int len = sizeof(Record);
	while(1){
		init(query);

		printf("\nEnter the domain name: ");scanf(" %[^\n]", query->domain);

		if (strcmp(query->domain, "end") == 0)
			break;
		//Send requested domain name
		sendto(sockfd, query->domain, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&server_address, sizeof(server_address));
		
		bzero(&buffer, sizeof(buffer));
		//Recieve IP address(es) of requested domain if it exists.
		recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&server_address, &len);

	    char* split = strtok(buffer, " ");
	    if(split){
	    	printf("\nThe IP Address of the requested domain is: ");
		    while(split){
		    	printf("\n%s", split);
		    	split = strtok(NULL, " ");
		    }
		    printf("\n");
		}
		else{
			printf("\nNo address in DNS.\n");
		}
	}

	close(sockfd);
}

/*
Output:
Client 1:
Enter the domain name: google.com

The IP Address of the requested domain is: 
192.168.1.1
17.10.23.123
255.254.253.252

Enter the domain name: youtube.com

The IP Address of the requested domain is: 
111.234.15.1

Enter the domain name: end

Client 2:
Enter the domain name: youtube.com

The IP Address of the requested domain is: 
111.234.15.1

Enter the domain name: yahoo.com

The IP Address of the requested domain is: 
194.12.34.12

Enter the domain name: end

Client 3:
Enter the domain name: yahoo.com

The IP Address of the requested domain is: 
194.12.34.12

Enter the domain name: google.com

The IP Address of the requested domain is: 
192.168.1.1
17.10.23.123
255.254.253.252

Enter the domain name: end


*/