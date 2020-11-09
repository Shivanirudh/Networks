//
//  pclient.c
//  
//
//  Created by Sharvan Kumaran on 11/9/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    int len;
    int sockfd;
    struct sockaddr_in servaddr,cliaddr;
    char buff[1024];
    sockfd  = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd<0)
        perror("Cannot create socket");
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(7228);
    
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    
    while(1)
    {
        printf("\nClient: ");
        scanf(" %[^\n]", buff);
        write(sockfd, buff,sizeof(buff));
        if (strcmp(buff, "bye") == 0)
            break;
        bzero(buff,sizeof(buff));
        read(sockfd,buff,sizeof(buff));
        printf("Server: %s\n",buff);
    }
    close(sockfd);
    return 0;
}