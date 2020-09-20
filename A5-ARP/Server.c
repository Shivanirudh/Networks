/*Simulation of ARP protocol*/
//Server

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>

struct Packet{
	char *sip;           //Source IP address
	char *smac;          //Source MAC address
	char *dip;           //Destination IP address
	char *dmac;          //Destination MAC address
	char *arp_packet;    //ARP packet
	char *data;          //Data 
};

typedef struct Packet Arp;

int main(int argc,char **argv){
	//Server and Client addresses
	struct sockaddr_in server_address, client_address;
	//Buffer to handle messages
	char buffer[1024];
	//Storing sockets for client 
	int client_sockets[30];
	//Set of file descriptors
	fd_set clientfds;
	//Socket file descriptor for accepting connections
	int newfd;
	
	//ARP Packet structure
	Arp packet;
	
	packet.sip = (char*)calloc(100,sizeof(char));
	packet.smac = (char*)calloc(100, sizeof(char));
	packet.dip = (char*)calloc(100,sizeof(char));
	packet.dmac = (char*)calloc(100, sizeof(char));
	packet.arp_packet = (char*)calloc(100, sizeof(char));
	packet.data = (char*)calloc(100, sizeof(char));

	//Accepting packet details
	printf("\nEnter the details of packet received. \n");
	printf("\nSource IP address: ");scanf(" %s", packet.sip);
	printf("\nSource MAC address: ");scanf(" %s", packet.smac);
	printf("\nDestination IP address: ");scanf(" %s", packet.dip);
	printf("\n16 Bit data: ");scanf(" %s", packet.data);
	

	//Developing ARP request packet
	printf("\nDeveloping ARP packet details.\n");
	strcpy(packet.arp_packet, packet.sip);strcat(packet.arp_packet, "|");
	strcat(packet.arp_packet, packet.smac);strcat(packet.arp_packet, "|");
	strcat(packet.arp_packet, packet.dip);
	printf("%s\n", packet.arp_packet);


	for(int i = 0; i < 30; i++)
		client_sockets[i] = 0;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);//domain = IPv4, type = TCP, protocol = IP
	if(sockfd < 0)
		perror("Error: Unable to create socket");

	//Filling server_address with null bytes
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;// Uses the Internet address family
	server_address.sin_addr.s_addr = INADDR_ANY;// Use any of the available addresses
	server_address.sin_port = htons(4500);// Connect to specified port 4500

	//Bind socket to the specified port
	if(bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address))<0)
		perror("Bind error");

	//Look for clients to serve, with a maximum limit of 5.
	listen(sockfd, 5);

	//New socket file descriptor to handle connections.
	int len = sizeof(client_address);
	while(1){
		//Clears socket set
		FD_ZERO(&clientfds);

		//Add main socket to the set
		FD_SET(sockfd, &clientfds);
		int max_sd = sockfd;

		//Adding valid secondary sockets to the set
		for(int i = 0;i < 30;i++){
			int sd = client_sockets[i];
			//Checking validity
			if(sd > 0)
				FD_SET(sd, &clientfds);
			

			//Store highest valued file descriptor
			if(sd > max_sd)
				max_sd = sd;
		}

		//Wait indefinitely for action on one of the sockets
		int action = select(max_sd + 1, &clientfds, NULL, NULL, NULL);
		if(action<0){
			perror("\nSelect error!\n");
		}

		//A change in main socket descriptor value implies that it is an incoming connection request
		if(FD_ISSET(sockfd, &clientfds)){
			newfd = accept(sockfd, (struct sockaddr*)&client_address, &len);
			if(newfd < 0)
				perror("\nUnable to accept new connection.\n");
						
			strcpy(buffer, packet.arp_packet);
			write(newfd, buffer, sizeof(buffer));
			//Add new client socket to list of sockets
			for(int i =0;i<30;i++){
				if(client_sockets[i] == 0){
					client_sockets[i] = newfd;
					break;
				}
			}
		}
		//Broadcasting on an established connection
		for(int i = 0;i<30;i++){
			int sd = client_sockets[i];
			bzero(buffer, sizeof(buffer));
			//Check for change in descriptors
			if(FD_ISSET(sd, &clientfds)){
				read(sd, buffer, sizeof(buffer));

				//Check ARP response
				if(buffer[0]){
					printf("\nARP Reply received: %s\n", buffer);
					int count = 0, k = 0;
					for(int i =0; buffer[i];i++){
						if(count == 3)
							packet.dmac[k++] = buffer[i];
						if(buffer[i] == '|')
							count++;
					}
					packet.dmac[k] = '\0';


					printf("\nSending packet to %s\n", packet.dmac);
					bzero(buffer, sizeof(buffer));
					
					//Write message in buffer
					strcpy(buffer, packet.arp_packet);strcat(buffer, "|");
					strcat(buffer, packet.dmac); strcat(buffer, "|");
					strcat(buffer, packet.data);

					write(newfd, buffer, sizeof(buffer));
					printf("\nPacket Sent: %s\n", buffer);
				}

			}
		}

	}
	
	return 0;
}
/*
Output:
Enter the details of packet received. 

Source IP address: 123.128.34.56

Source MAC address: AF-45-E5-00-97-12

Destination IP address: 155.157.65.128

16 Bit data: 1011110000101010

Developing ARP packet details.
123.128.34.56|AF-45-E5-00-97-12|155.157.65.128

ARP Reply received: 123.128.34.56|AF-45-E5-00-97-12|155.157.65.128|45-D4-62-21-1A-B2

Sending packet to 45-D4-62-21-1A-B2

Packet Sent: 123.128.34.56|AF-45-E5-00-97-12|155.157.65.128|45-D4-62-21-1A-B2|1011110000101010

*/
