#include "ARPPacket.h"

int main(int argc, char **argv){
    if(argc < 2){
        perror("Server ip address needed.");
        exit(1);
    }

    struct sockaddr_in server;
    char buffer[1024];

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Error: Socket");
        exit(1);
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(7882);

    Packet arp;
    init(&arp);
    acceptPacket(&arp);
    while(!checkPacket(&arp)){
        printf("\nInvalid packet. Please re-enter");
        acceptPacket(&arp);
    }
    printf("\nThe packet to be sent is:\n");
    printPacket(&arp);
    strcpy(buffer, developPacket(&arp));
    sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr*)&server, sizeof(server));
    bzero(&buffer, sizeof(buffer));
    recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&server, sizeof(server));
    printf("\nRecieved msg is %s\n", buffer);
}