/*
i. Write a socket program to perform the following using UDP.
a.Server maintains the mapping of webpage with the ip address.
b.Client sends the request for a webpage.
c.Server checks the table and responds with the appropriate ip address.
d.if not found send an error message to the client.

CLIENT PART
*/

#include "table.h"

int main(int argc, char **argv){

    if(argc < 2){
        perror("\n Error: Client needs server IP address.");
        exit(0);
    }

    struct sockaddr_in server_address;
    char buffer[1024];
    Record *query = (Record*)malloc(sizeof(Record));

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0)
        perror("\n Error: Unable to create connection");
    
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;   //IP family
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);      //Any free address
    server_address.sin_port = htons(5555);    //Port number 5555

    int len = sizeof(Record);
    while(1){
        initialize(query);

        printf("\n Enter webpage name: ");scanf(" %[^\n]", query->domain);
        if(strcmp(query->domain, "end") == 0){
            break;
        }

        //Request for webpage
        sendto(sockfd, query->domain, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&server_address, sizeof(server_address));

        bzero(&buffer, sizeof(buffer));
        recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&server_address, &len);
        char* split = strtok(buffer, " ");
	    if(split){
	    	printf("\n The IP Address of the requested webpage is: ");
		    while(split){
		    	printf("\n %s", split);
		    	split = strtok(NULL, " ");
		    }
		    printf("\n");
		}
		else{
			printf("\n No address for requested webpage in table.\n");
		}
	}

	close(sockfd);
}