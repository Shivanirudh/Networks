/*Chat Server Client Program*/
//Client

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>

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
	//Chat session ends if buffer contains "end"
	while(strcmp(buffer, "end") != 0){
		bzero(buffer, sizeof(buffer));
		read(sockfd, buffer, sizeof(buffer));
		printf("\nMessage from Server: %s", buffer);

		bzero(buffer, sizeof(buffer));
		//Write message in buffer
		printf("\nEnter the message: ");scanf(" %[^\n]", buffer);
		write(sockfd, buffer, sizeof(buffer));
	}
	printf("\nSession end");
	close(sockfd);
	return 0;
}

/*
Output:
Client 1:
Message from Server: Connection established
Enter the message: hi

Message from Server: hello client 4
Enter the message: department
end

Message from Server: end
Enter the message: end
Session end

Client 2:
Message from Server: Connection established
Enter the message: hi server

Message from Server: hlo client 5
Enter the message: ssn    

Message from Server: cse
Enter the message: end

Session end
*/