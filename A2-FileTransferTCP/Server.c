/*File Transfer Program*/
//Server

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>

int main(int argc,char **argv){
	//Server and Client addresses
	struct sockaddr_in server_address, client_address;
	//Buffer to handle messages
	char buffer[1024];
	
	//Socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);//domain = IPv4, type = TCP, protocol = IP
	if(sockfd < 0)
		perror("Error: Unable to create socket");

	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;// Uses the Internet address family
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

	//Read file name from buffer
	read(newfd, buffer, sizeof(buffer));
	printf("\nFile to be transferred to Client: %s\n", buffer);

	int sourcefd = open(buffer, O_RDONLY);
	if(sourcefd == -1){
		printf("\nNo source file");
	}
	else{
		bzero(buffer, sizeof(buffer));
		//Read file contents
		int readfd = read(sourcefd, buffer, sizeof(buffer));
		buffer[readfd] = '\0';
		//Send file contents back to client
		write(newfd, buffer, sizeof(buffer));
		close(sourcefd);
	}
	printf("\n File successfully transferred\n");
	close(sockfd);
	close(newfd);
	return 0;
}
/*
Output:

File to be transferred to Client: ./source.txt

File successfully transferred

*/