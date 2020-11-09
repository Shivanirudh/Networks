//
//  pserver.c
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

int main()
{
    int sockfd;
    int len;
    struct sockaddr_in servaddr,cliaddr;
    char buff[1024];
    int newfd;
    pid_t childpid;
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
        perror("Cannot create socket");
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(7228);
    
    if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
    {
        printf("bind error\n");
        exit(1);
    }
    listen(sockfd,10);
    len = sizeof(cliaddr);
    while(1)
    {
        newfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
        if(newfd<0)
        {
            exit(1);
        }
        int num = ntohs(cliaddr.sin_port) % 100;
        printf("Client %d has entered the chat\n",num);
        if((childpid = fork())==0)
        {
            close(sockfd);
            while(1)
            {   bzero(buff, sizeof(buff));
                read(newfd, buff, sizeof(buff));
                if (strcmp(buff, "bye") == 0)
                {
                    printf("Client %d Left\n", num);
                    break;
                }
                else
                {
                    printf("Client %d: %s", num, buff);
                    bzero(buff, sizeof(buff)); 
                    printf("\nServer: ");
                    scanf(" %[^\n]", buff);
                    write(newfd, buff, sizeof(buff));
                }
                
            }
            close(newfd);
        }
        
    }
    close(newfd);
    return 0;
}
