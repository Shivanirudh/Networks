/*Day time Server*/
//Client

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

int main(int argc, char **argv){
	//Server and Client addresses
	struct sockaddr_in server_address;
	//Buffer to handle messages
	char buffer[1024];

	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //domain = IPv4, type = UDP, protocol = ip
	if(sockfd < 0){
		perror("\nError: Unable to create socket.");
	}

	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family    = AF_INET; // Uses Internet adress family
    server_address.sin_addr.s_addr = INADDR_ANY; //Use any of the available addresses
    server_address.sin_port = htons(5678); //Use port 5678

    int choice;
    char option;

    do{
    	//Read option
    	printf("\n Choose option: \n 1. Date \n 2. Day \n 3. Month \n 4. Year \n 5. Time \n Your choice: ");
    	scanf("%d", &choice);
    	//Converting option to string
    	snprintf(buffer, 10, "%d", choice);
	    
	    //Sending request to server
	    sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr*)&server_address, sizeof(server_address));
	    
	    //Read response from buffer
	    recvfrom(sockfd, buffer, sizeof(buffer),  MSG_WAITALL, (struct sockaddr*)&server_address, sizeof(server_address));
	    printf("\n%s\n", buffer);

	    printf("\n Do you want to continue?(y/n) "); scanf(" %c", &option);
	}while(option == 'y' || option == 'Y');
    close(sockfd);
}

/*
Output:

Client 1:
 Choose option: 
 1. Date 
 2. Day 
 3. Month 
 4. Year 
 5. Time 
 Your choice: 5

The time is 10:3:39

 Do you want to continue?(y/n) y

 Choose option: 
 1. Date 
 2. Day 
 3. Month 
 4. Year 
 5. Time 
 Your choice: 3

The month is September

 Do you want to continue?(y/n) n

Client 2:
 Choose option: 
 1. Date 
 2. Day 
 3. Month 
 4. Year 
 5. Time 
 Your choice: 2

The day is Sunday

 Do you want to continue?(y/n) y

 Choose option: 
 1. Date 
 2. Day 
 3. Month 
 4. Year 
 5. Time 
 Your choice: 4

The year is 2020

 Do you want to continue?(y/n) y

 Choose option: 
 1. Date 
 2. Day 
 3. Month 
 4. Year 
 5. Time 
 Your choice: 1

The date is 13/9/2020

 Do you want to continue?(y/n) n
*/