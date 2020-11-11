#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

int main(int argc,char **argv)
{
int len,n;
int sockfd;
struct sockaddr_in servaddr,cliaddr;
char buff[1024];

sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(sockfd<0)
	perror("cannot create socket");

bzero(&servaddr,sizeof(servaddr));
bzero(&cliaddr,sizeof(cliaddr));

servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=INADDR_ANY;
servaddr.sin_port=htons(5678);

if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	perror("Bind error");

while(1)
{
len=sizeof(cliaddr);
n = recvfrom(sockfd, (char *)buff, sizeof(buff),  MSG_WAITALL, ( struct sockaddr*) &cliaddr,&len); 
int num=ntohs(cliaddr.sin_port)%100;
buff[n] = '\0'; 
printf("Client %d: %s\n",num, buff); 
printf("Enter message:");
scanf("%[^\n]%*c",buff);
sendto(sockfd, (char *)buff, sizeof(buff), MSG_CONFIRM, (struct sockaddr *) &cliaddr, len); 
}
close(sockfd);
return 0;
}