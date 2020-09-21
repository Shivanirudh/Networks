/*Simulation of ARP Protocol*/
//Client

#include "Packet.h"

int main(int argc,char** argv){
	//Server and client addresses
	struct sockaddr_in server_address, client_address;
	//Buffer to handle messages
	char buffer[1024];
	
	//Check arguments
	if(argc<2){
		perror("\nError: IP address to be passed in command line.\n");
	}

	//ARP Packet structure
	ARP packet;
	
	//Initialising ARP packet
	init(&packet);
	
	//Accepting addresses
	printf("\nEnter IP address: ");scanf(" %s", packet.sip);
	printf("\nEnter MAC address: ");scanf(" %s", packet.smac);

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

	bzero(buffer, sizeof(buffer));
	read(sockfd, buffer, sizeof(buffer));
	printf("\nARP Request received: %s\n", buffer);

	int count = 0, k = 0;
	for(int i =0; buffer[i];i++){
		if(count == 2)
			packet.dip[k++] = buffer[i];
		if(buffer[i] == '|')
			count++;
	}
	packet.dip[k] = '\0';

	//Check ARP request packet
	if(strcmp(packet.dip, packet.sip) == 0){
		printf("\nIP address matches.\n");
		//Write message in buffer
		strcat(buffer, "|");strcat(buffer, packet.smac);
		write(sockfd, buffer, sizeof(buffer));
		printf("\nARP reply sent: %s\n", buffer);

		bzero(buffer, sizeof(buffer));
		read(sockfd, buffer, sizeof(buffer));
		printf("\nPacket received: %s\n", buffer);
	}
	else{
		printf("\nIP address does not match.\n");
	}
	
	close(sockfd);
	return 0;
}

/*
Output:
Client 1:
Enter IP address: 165.43.158.158

Enter MAC address: 09-DF-90-26-6C-09

ARP Request received: 123.128.34.56|AF-45-E5-00-97-12|155.157.65.128

IP address does not match.

Client 2:
Enter IP address: 155.157.65.128

Enter MAC address: 45-D4-62-21-1A-B2

ARP Request received: 123.128.34.56|AF-45-E5-00-97-12|155.157.65.128

IP address matches.

ARP reply sent: 123.128.34.56|AF-45-E5-00-97-12|155.157.65.128|45-D4-62-21-1A-B2

Packet received: 123.128.34.56|AF-45-E5-00-97-12|155.157.65.128|45-D4-62-21-1A-B2|1011110000101010

Client 3:
Enter IP address: 15.143.158.18

Enter MAC address: 19-0F-01-63-C7-D4

ARP Request received: 123.128.34.56|AF-45-E5-00-97-12|155.157.65.128

IP address does not match.

*/