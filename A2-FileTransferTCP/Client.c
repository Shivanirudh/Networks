/*File Transfer Program*/
//Client

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>

int main(int argc,char** argv){
	//Server and client addresses
	struct sockaddr_in server_address, client_address;
	//Buffer to handle messages
	char buffer[1024];
	
	//Server socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);//(domain = Ipv4, type = TCP, protocol = 0
	if(sockfd < 0)
		perror("Error: Unable to create socket");
	
	//Filling server address with null bytes
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;//Use the Internet address family
	server_address.sin_addr.s_addr = inet_addr(argv[1]);//Use ip address passed as command line argument
	server_address.sin_port = htons(4500);//Connect socket to port 4500
	
	//Attempt to connect client to socket on specified port
	connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));

	int len = sizeof(client_address);
	
	//Write file name into buffer
	printf("Enter the path of file: ");scanf(" %[^\n]", buffer);
	write(sockfd, buffer, sizeof(buffer));

	//Read file contents from buffer
 	read(sockfd, buffer, sizeof(buffer));
	printf("\nFile contents: %s\n", buffer);

	printf("\nFile successfully received\n");
	//Write file contents to new file
	int destfd = open("dest.txt", O_RDWR|O_CREAT);
	if(destfd == -1){
		printf("\nUnable to create destination file\n");
	}
	else{
		int writefd = write(destfd, buffer, sizeof(buffer));
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