//Hamming code using TCP
//Server

#include "Hamming.h"

int main(int argc, char **argv){
    if(argc > 1){
        perror("Error:No need arguments for server");
        exit(1);
    }
    struct sockaddr_in server, client;
    pid_t child;
    char buffer[1024];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("\nError: Socket");
        exit(1);
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(7000);
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("Error: Bind");
        exit(1);
    }

    listen(sockfd, 5);
    int len = sizeof(client);
    
    while(1){
        int newfd = accept(sockfd, (struct sockaddr*)&client, &len);
        if(newfd < 0){
            perror("Error:Accept");
            exit(1);
        }
        socklen_t port;
        struct sockaddr_in addr;
        int res = getpeername(newfd, (struct sockaddr*)&addr, &port);
        printf("\nClient %d", ntohs(port));

        child = fork();
        if(child == 0){
            while(1){
                read(newfd, buffer, sizeof(buffer));
                printf("\nTransmitted data: %s\n", buffer);

                int error_pos = compute_error_pos(buffer, 0);
                
                if(error_pos>0){
                    printf("\nError at %d bit", error_pos);
                    strrev(buffer);
                    char c = buffer[error_pos - 1];
                    buffer[error_pos-1] = c == '1'? '0':'1';
                    strrev(buffer);
                    printf("\nAfter correction: %s", buffer);
                }
                strcpy(buffer, decode(buffer));

                printf("\nOriginal data: %s\n", buffer);
                write(newfd, buffer, sizeof(buffer));
            }
            close(newfd);
        }
    }

}

/*
Output:
Client 4096
Transmitted data: 10101101110

Error at 6 bit
After correction: 10101001110
Original data: 1011001
*/