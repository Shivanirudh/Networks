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

typedef struct Packet ARP;

void init(ARP* packet){
	packet->sip = (char*)calloc(100,sizeof(char));
	packet->smac = (char*)calloc(100, sizeof(char));
	packet->dip = (char*)calloc(100,sizeof(char));
	packet->dmac = (char*)calloc(100, sizeof(char));
	packet->arp_packet = (char*)calloc(100, sizeof(char));
	packet->data = (char*)calloc(100, sizeof(char));
}

void acceptPacket(ARP *packet){
	printf("\nEnter the details of packet received. \n");
	printf("\nSource IP address: ");scanf(" %s", packet->sip);
	printf("\nSource MAC address: ");scanf(" %s", packet->smac);
	printf("\nDestination IP address: ");scanf(" %s", packet->dip);
	printf("\n16 Bit data: ");scanf(" %s", packet->data);
}

void developPacket(ARP *packet){
	printf("\nDeveloping ARP packet details.\n");
	strcpy(packet->arp_packet, packet->sip);strcat(packet->arp_packet, "|");
	strcat(packet->arp_packet, packet->smac);strcat(packet->arp_packet, "|");
	strcat(packet->arp_packet, packet->dip);
}

void get_destmac(ARP *packet, char* buffer){
	int count = 0, k = 0;
	for(int i =0; buffer[i];i++){
		if(count == 3)
			packet->dmac[k++] = buffer[i];
		if(buffer[i] == '|')
			count++;
	}
	packet->dmac[k] = '\0';
}

void develop_msg(ARP *packet, char* buffer){
	strcpy(buffer, packet->arp_packet);strcat(buffer, "|");
	strcat(buffer, packet->dmac); strcat(buffer, "|");
	strcat(buffer, packet->data);
}