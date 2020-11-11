#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char **argv)
{
int len,n;
int sockfd,newfd;
struct sockaddr_in servaddr;
char buff[1024];

sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(sockfd<0)
	perror("cannot create socket");

bzero(&servaddr,sizeof(servaddr));

servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr(argv[1]);
servaddr.sin_port=htons(5678);

printf("Enter message:");
scanf("%[^\n]%*c",buff);

while(strncmp(buff,"exit",4)!=0)
{
	sendto(sockfd, (char *)buff, sizeof(buff), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr)); 

	n = recvfrom(sockfd, (char *)buff, sizeof(buff),  MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
	buff[n] = '\0'; 
	if(strncmp(buff,"exit",4)==0)
		break;
	printf("Server : %s\n", buff); 

	printf("Enter message:");
	scanf("%[^\n]%*c",buff);
}
printf("Disconnected from server");
close(sockfd);
return 0;
}