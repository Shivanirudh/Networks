/*
1. Write a socket program for simple client server connectivity using TCP.
2. Client will send an ip address (classful addressing). Server will find, to which class it belongs to for classful addressing, the default mask value and sends it back to client.
3. Client will send an ip address with prefix length (classless addressing). Server will find the first address and last address of the network.
4. Server will find how many host machines can be connected to this ip address.
*/

#include "Addressing.h"

int main(int argc, char **argv){
    if(argc != 2){
        perror("\nError: Server IP address needed for client");
        exit(1);
    }
    struct sockaddr_in server;
    char buffer[1024];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("\nError: Socket");
        exit(1);
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(7002);
    server.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));
    while(1){
        printf("\n------------------------------------------------\n");
        bzero(&buffer, sizeof(buffer));
        printf("\nEnter an IP address: ");scanf(" %[^\n]", buffer);
        if(!strcmp(buffer, "end")){
            write(sockfd, buffer, sizeof(buffer));
            break;
        }
        while(!checkIP(buffer)){
            printf("\nInvalid IP address. Please re-enter: ");
            scanf(" %[^\n]", buffer);
        }
        write(sockfd, buffer, sizeof(buffer));
        
        bzero(&buffer, sizeof(buffer));

        read(sockfd, buffer, sizeof(buffer));
        char *token = strtok(buffer, " ");
        while(token){
            if(!strcmp(token, "1")){
                token = strtok(NULL, " ");
                printf("\nClass: %s", token);
                token = strtok(NULL, " ");
                if(token[0])
                    printf("\nDefault mask: %s\n", token);
                else
                    printf("\nClasses D and E do not have default masks.\n");
                token = strtok(NULL, " ");
            }
            else{
               token = strtok(NULL, " ");
                printf("\nFirst address: %s", token);
                token = strtok(NULL, " ");
                printf("\nLast address: %s\n", token);
                token = strtok(NULL, " ");
                printf("\nNumber of hosts: %s\n", token);
                token = strtok(NULL, " ");
            }
        }
        printf("\n------------------------------------------------\n");
        
    }
    close(sockfd);
}