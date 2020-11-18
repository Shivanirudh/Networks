//Hamming code using TCP
//Client

#include "Hamming.h"

int main(int argc, char **argv){
    if(argc != 2){
        perror("Error:Server ip needed for client");
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
    server.sin_port = htons(7000);
    server.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    while(1){
        int pos = 0;
        printf("\nEnter input data: ");scanf(" %[^\n]", buffer);
        printf("Enter position for error: ");scanf("%d", &pos);
        strcpy(buffer, even_parity(buffer));
        if(pos >=0 ){
            strrev(buffer);
            char c = buffer[pos];
            buffer[pos - 1] = c == '1'? '0':'1';
            strrev(buffer);
        }
        printf("\nTransmitted data:%s", buffer);
        write(sockfd, buffer, sizeof(buffer));

        bzero(&buffer, sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer));
        printf("\nCorrect data: %s", buffer);
    }
}

/*
Output:
Enter input data: 1011001
Enter position for error: 6

Transmitted data: 10101101110
Correct data: 1011001
**/