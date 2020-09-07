/*Simple Server Client program*/
//Client part
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
int main(int argc,char** argv){
	
	//Server and client addresses
	struct sockaddr_in servaddr, cliaddr;
	//Buffer to handle messages
	char buff[1024];
	
	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); //(domain = Ipv4, type = TCP, protocol = 0)
	if(sockfd < 0)
		perror("cannotcreate socket");
	
	//Filling server address with null bytes
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET; //Use the Internet address family
	servaddr.sin_addr.s_addr = inet_addr(argv[1]); //Use ip address passed as command line argument
	servaddr.sin_port = htons(7228); //Connect socket to port 7228

	//Attempt to connect client to socket on specified port
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	//Write message into buffer
	printf("Enter the message: ");
	scanf(" %[^\n]", buff);
	write(sockfd, buff, sizeof(buff));

	close(sockfd);
	return 0;
}