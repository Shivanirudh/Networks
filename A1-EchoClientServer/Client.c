/*Echo Server Client Program*/
//Client

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char** argv){
	//Server and client addresses
	struct sockaddr_in serveraddr, clientaddr;
	//Buffer to handle messages
	char buffer[1024];
	
	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);//(domain = Ipv4, type = TCP, protocol = 0
	if(sockfd < 0)
		perror("Error: Unable to create socket");
	
	//Filling server address with null bytes
	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;//Use the Internet address family
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);//Use ip address passed as command line argument
	serveraddr.sin_port = htons(4500);//Connect socket to port 4500

	//Attempt to connect client to socket on specified port
	connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	//Sending Message
	int len = sizeof(clientaddr);
	
	//Write message into buffer
	printf("Enter the message: ");scanf(" %[^\n]", buffer);
	write(sockfd, buffer, sizeof(buffer));
	
	//Read echoed message from buffer
	read(sockfd, buffer, sizeof(buffer));
	printf("Message from Server: %s", buffer);
	
	close(sockfd);
	return 0;
}

/*
Output:

Enter the message: this is from ssn cse 
Message from Server: this is from ssn cse 
*/