#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main()
{

    int sockfd, ret;
    struct sockaddr_in servaddr;
    int newfd;
    struct sockaddr_in cliaddr;
    socklen_t addr_size;
    char buff[1024];
    pid_t childpid;
    int len;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("cannot create socket");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;         //uses the internet address family
    servaddr.sin_port = htons(7228);       //connects sd to port 7726
    servaddr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY used when you don't need to bind to a specific IP

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind error");
        exit(1);
    }

    listen(sockfd, 10);
    len = sizeof(cliaddr);

    while (1)
    {
        newfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
        if (newfd < 0)
        {
            exit(1);
        }
        //cliaddr.sin_port is unique so can be used to recognize the client
        int num = ntohs(cliaddr.sin_port) % 100;
        printf("Client %d joined the chat\n", num);

        if ((childpid = fork()) == 0)
        {
            close(sockfd);
            while (1)
            {
                read(newfd, buff, sizeof(buff));
                if (strcmp(buff, "bye") == 0) //when the client enters bye, the client leaves
                {
                    printf("Client %d Left\n", num);
                    break;
                }
                else
                {
                    printf("Client %d: %s", num, buff);
                    bzero(buff, sizeof(buff)); //to empty the buff
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