/*Echo Server Client Program*/
//Server

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char **argv){
	int len;
	int sockfd, newfd, n;
	struct sockaddr_in serveraddr, clientaddr;
	char buffer[1024];
	char str[1000];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		perror("Error: Unable to create socket");

	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(4500);

	if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))<0)
		perror("Bind error");

	listen(sockfd, 2);

	len = sizeof(clientaddr);
	newfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

	//Receiving the message
	n = read(newfd, buffer, sizeof(buffer));
	printf("\nMessage from Client: %s\n", buffer);

	n = write(newfd, buffer, sizeof(buffer));
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