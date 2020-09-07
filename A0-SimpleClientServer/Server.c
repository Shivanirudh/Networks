/*Simple Server Client program*/
//Server part
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char **argv){

	//Server and client addresses
	struct sockaddr_in servaddr, cliaddr;
	//Buffer to handle messages
	char buff[1024];

	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); //(domain = IPv4, type = TCP, protocol = IP)
	if(sockfd < 0)
		perror("Error: Unable to create socket");

	//Sets servaddr with null bytes
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET; //Uses the Internet address family
	servaddr.sin_addr.s_addr = INADDR_ANY; //Use any of the available addresses
	servaddr.sin_port = htons(7228); //Connect socket to port 7228

	//Bind socket to the specified port
	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0)
		perror("Binderror");

	//Look for clients to serve, with a maximum limit of 2. 
	listen(sockfd, 2);

	//New socket file descriptor to handle connections.
	int len = sizeof(cliaddr);
	int newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
	
	//Read message from buffer
	read(newfd, buff, sizeof(buff));
	printf("\nReceived Message is\t%s\n", buff);

	close(sockfd);
	close(newfd);
	return 0;
}