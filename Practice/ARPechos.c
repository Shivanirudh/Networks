#include "ARPPacket.h"

int main(int argc, char **argv){
    if(argc > 1){
        perror("No argument needed for server program.");
        exit(1);
    }

    struct sockaddr_in server, client;
    char buffer[1024];

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
        perror("Error: Socket");
        exit(1);
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(7882);

    if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("Error: Bind");
        exit(1);
    }

    int len = sizeof(client);
    recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&client, &len);
    printf("\nReceived message: \n%s\n", buffer);
    sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr*)&client, len);
}