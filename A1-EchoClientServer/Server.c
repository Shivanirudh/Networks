/*Echo Server Client Program*/
//Server

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char **argv){
	//Server and Client addresses
	struct sockaddr_in server_address, client_address;
	//Buffer to handle messages
	char buffer[1024];

	//Socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); //domain = IPv4, type = TCP, protocol = IP
	if(sockfd < 0)
		perror("Error: Unable to create socket");

	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET; // Uses the Internet address family
	server_address.sin_addr.s_addr = INADDR_ANY;// Use any of the available addresses
	server_address.sin_port = htons(4500);// Connect to specified port 4500

	//Bind socket to the specified port
	if(bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address))<0)
		perror("Bind error");

	//Look for clients to serve, with a maximum limit of 2. 
	listen(sockfd, 2);

	//New socket file descriptor to handle connections.
	int len = sizeof(client_address);
	int newfd = accept(sockfd, (struct sockaddr*)&client_address, &len);

	//Read message from buffer
	read(newfd, buffer, sizeof(buffer));
	printf("\nMessage from Client: %s\n", buffer);

	//Echo message back to client
	write(newfd, buffer, sizeof(buffer));
	printf("\nMessage sent: %s\n", buffer);

	close(sockfd);
	close(newfd);
	return 0;
}
/*
Output:
Message from Client: hi this is from ssn cse

Message sent: hi this is from ssn cse
*/