/*
1. Write a socket program for simple client server connectivity using TCP.
2. Client will send an ip address (classful addressing). Server will find, to which class it belongs to for classful addressing, the default mask value and sends it back to client.
3. Client will send an ip address with prefix length (classless addressing). Server will find the first address and last address of the network.
4. Server will find how many host machines can be connected to this ip address.
*/

//Server

#include "Addressing.h"

int main(int argc, char **argv){
    if(argc > 1){
        perror("\nError: No arguments needed for server");
        exit(1);
    }
    
    struct sockaddr_in server, client;
    char buffer[1024];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("\nError: Socket");
        exit(1);
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(7002);
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("\nError:Bind");
        exit(1);
    }

    listen(sockfd, 2);

    int len = sizeof(client);

    int newfd = accept(sockfd, (struct sockaddr *)&client, &len);
    
    while(1){
        bzero(&buffer, sizeof(buffer));

        read(newfd, buffer, sizeof(buffer));
        if(!strcmp(buffer, "end")){
            break;
        }
        printf("\n------------------------------------------------\n");
        printf("\nReceived IP address: %s\n", buffer);

        int cat = find_cat(buffer);

        //Classful addressing
        if(cat){
            char *class_name = (char*)calloc(100, sizeof(char));
            strcpy(class_name, find_class(buffer));
            char *default_mask = (char*)calloc(100, sizeof(buffer));
            strcpy(default_mask, find_def_mask(class_name, buffer));

            bzero(&buffer, sizeof(buffer));
            strcat(buffer, "1");strcat(buffer, " ");
            strcat(buffer, class_name);strcat(buffer, " ");
            strcat(buffer, default_mask);
        }
        //Classless addressing
        else{
            char *first_addr = (char*)calloc(100, sizeof(char));
            char *last_addr = (char*)calloc(100, sizeof(char));
            strcpy(first_addr, find_first(buffer));
            strcpy(last_addr, find_last(buffer));
            char *hosts_count = (char*)calloc(100, sizeof(char));
            strcpy(hosts_count, find_hosts(buffer));

            bzero(&buffer, sizeof(buffer));
            strcat(buffer, "0");strcat(buffer, " ");
            strcat(buffer, first_addr);strcat(buffer, " ");
            strcat(buffer, last_addr);strcat(buffer, " ");
            strcat(buffer, hosts_count);strcat(buffer, " ");
        }
        printf("\nMessage to be sent: %s\n", buffer);
        write(newfd, buffer, sizeof(buffer));
        printf("\n------------------------------------------------\n");    
    }
    close(newfd);
    close(sockfd);

}

