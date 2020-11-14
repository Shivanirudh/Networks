/*Write a socket program to implement Echo server using UDP.

At client side, read the details such as source IP address, Source MAC address, Destination IP address and 16 bit data.
Develop an ARP Request packet and transmit to the server.
The server reads the ARP request packet and echo back to the client.
Add two more clients to transmit the ARP request packet. The server should echo to the corresponding client.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

struct ARP{
    char *sip;
    char *smac;
    char *dip;
    char *dmac;
    char *data;
};

typedef struct ARP Packet;

void init(Packet *p){
    p->sip = (char*)calloc(100, sizeof(char));
    p->smac = (char*)calloc(100, sizeof(char));
    p->dip = (char*)calloc(100, sizeof(char));
    p->dmac = (char*)calloc(100, sizeof(char));
    p->data = (char*)calloc(100, sizeof(char));
}

char* developPacket(Packet *p){
    char *packet = (char*)calloc(100, sizeof(char));
    strcpy(packet, p->sip);strcat(packet, "|");
    strcat(packet, p->dip);strcat(packet, "|");
    strcat(packet, p->smac);strcat(packet, "|");
    strcat(packet, p->dmac);strcat(packet, "|");
    strcat(packet, p->data);
    return packet;
}

void printPacket(Packet *p){
    char *packet = developPacket(p);
    printf("\n%s\n", packet);
}

int checkIP(char *ip){
    char *addr = (char*)calloc(100, sizeof(char));
    strcpy(addr, ip);

    int count = 0;
    for(int i = 0; addr[i]; i++){
        if(addr[i] == '.')
            count++;
    }
    if(count != 3)
        return 0;

    char *token = strtok(addr, ".");
    while(token){
        int val = atoi(token);
        if(val < 0 || val > 255)
            return 0;
        token = strtok(NULL, ".");
    }
    return 1;
}

int checkMAC(char *mac){
    char *addr = (char*)calloc(100, sizeof(char));
    strcpy(addr, mac);

    int count = 0;
    int vcount = 0;
    for(int i = 0;addr[i];i++){
        if(addr[i] == ':'){
            if(vcount != 2){
                return 0;
            }
            else{
                vcount = 0;
            }
            count++;
        }
        vcount++;
    }
    if(count != 5)
        return 0;
    
    for(int i = 0; addr[i]; i++){
        if(addr[i] == ':')
            continue;
        else{
            int res = 0;
            if(addr[i] >= '0' && addr[i] <= '9')
                res = 1;
            else{
                switch(addr[i]){
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F': res = 1;
                    default: res = 0;
                }
            }
            if(!res)
                return 0;
        }
    }
    return 1;
}

char* fixData(char *data){
    char *str = (char*)calloc(100, sizeof(char));
    int dlen = strlen(data);
    dlen = 16-dlen;
    while(dlen--){
        strcat(str, "0");
    }
    strcat(str, data);
    return str;
}

void acceptPacket(Packet *p){
    printf("\nEnter source ip address: ");scanf(" %s", p->sip);
    printf("\nEnter source mac address: ");scanf(" %s", p->smac);
    printf("\nEnter destination ip address: ");scanf(" %s", p->dip);
    printf("\nEnter destination mac address: ");scanf(" %s", p->dmac);
    printf("\nEnter data: ");scanf(" %s", p->data);
    strcpy(p->data, fixData(p->data));
}

int checkPacket(Packet *p){
    return checkIP(p->sip) * checkIP(p->dip) * checkMAC(p->smac) * checkMAC(p->dmac);
}