#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char **argv)
{
    int len;
    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    char str[1000];
    char buff[1024];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("cannot create socket");
    //erases the data in sizeof(servaddr) bytes starting from servaddr
    bzero(&servaddr, sizeof(servaddr));

    //uses the internet address family
    servaddr.sin_family = AF_INET;
    //connects to the IP address passed as argument
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(7228); //connects sd to port 7726

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1)
    {
        printf("\nClient: ");
        scanf(" %[^\n]", buff);
        write(sockfd, buff, sizeof(buff));
        if (strcmp(buff, "bye") == 0)
            break;
        bzero(buff, sizeof(buff)); //to empty the buffer

        read(sockfd, buff, sizeof(buff));
        printf("\nServer: %s", buff);
    }
    close(sockfd);
    return 0;
}