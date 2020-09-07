/*Chat Server Client Program*/
//Client

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char** argv){
	int len;
	int sockfd, n;
	struct sockaddr_in serveraddr, clientaddr;
	char str[1000];
	char buffer[1024];
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		perror("Error: Unable to create socket");
	
	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(4500);

	if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))<0)
		perror("Bind error");
	
	connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	//Sending Message
	len = sizeof(clientaddr);

	while(strcmp(buffer, "end") != 0){
		//bzero(buffer, sizeof(buffer));
		
		printf("\nEnter the message: ");scanf(" %[^\n]", buffer);
		n = write(sockfd, buffer, sizeof(buffer));
		
		bzero(buffer, sizeof(buffer));
		
		n = read(sockfd, buffer, sizeof(buffer));
		printf("\nMessage from Server: %s", buffer);
	}
	printf("\nSession end");
	close(sockfd);
	return 0;
}

/*
Output:

Enter the message: this is from ssn cse 
Message from Server: this is from ssn cse 
*/