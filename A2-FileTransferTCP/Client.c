/*File Transfer Program*/
//Client

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>

int main(int argc,char** argv){
	int len;
	int sockfd, n;
	struct sockaddr_in serveraddr, clientaddr;
	char str[1000];
	char buffer[1024], file_cont[1024];
	
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
	
	printf("Enter the path of file: ");scanf(" %[^\n]", buffer);
	n = write(sockfd, buffer, sizeof(buffer));
	
	listen(sockfd, 2);
	printf("\nFile successfully received\n");

	int r = read(sockfd, file_cont, sizeof(file_cont));
	printf("\nFile contents: %s\n", file_cont);

	int destfd = open("dest.txt", O_RDWR|O_CREAT);
	if(destfd == -1){
		printf("\nUnable to create destination file\n");
	}
	else{
		int writefd = write(destfd, file_cont, sizeof(file_cont));
		close(destfd);
		printf("\nFile Saved at: dest.txt\n");
	}
	close(sockfd);
	return 0;
}

/*
Output:
Enter the path of file: ./source.txt

File successfully received

File contents: hi this is from ssn cse
hello world
123
456
789
000

File Saved at: ./dest.txt

*/