/*File Transfer Program*/
//Server

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>

int main(int argc,char **argv){
	int len;
	int sockfd, newfd, n;
	struct sockaddr_in serveraddr, clientaddr;
	char buffer[1024], file_cont[1024];
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

	printf("\nWaiting for Client...\n");
	listen(sockfd, 2);

	len = sizeof(clientaddr);
	newfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

	//Receiving the message
	n = read(newfd, buffer, sizeof(buffer));
	printf("\nFile to be transferred to Client: %s\n", buffer);

	int sourcefd = open(buffer, O_RDONLY);
	if(sourcefd == -1){
		printf("\nNo source file");
	}
	else{
		int readfd = read(sourcefd, file_cont, sizeof(file_cont));
		file_cont[readfd] = '\0';
		int w = write(newfd, file_cont, sizeof(file_cont));
		close(sourcefd);
	}
	printf("\n File successfully transferred\n");
	close(sockfd);
	close(newfd);
	return 0;
}
/*
Output:
Waiting for Client...

File to be transferred to Client: ./source.txt

File successfully transferred

*/