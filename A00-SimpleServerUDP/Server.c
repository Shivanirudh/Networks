/*Day time Server*/
//Server

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc, char **argv){
	//Server and Client addresses
	struct sockaddr_in server_address, client_address;
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

    //Bind socket to the specified port
	if(bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address))<0)
		perror("Bind error");

	int len = sizeof(client_address);
	recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&client_address, &len);
	printf("\nMessage from client: %s\n", buffer);
	
	printf("\nEnter a message: ");scanf(" %[^\n]", buffer);
	sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr*)&client_address, len);

	close(sockfd);
}