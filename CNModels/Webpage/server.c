/*
i. Write a socket program to perform the following using UDP.
a.Server maintains the mapping of webpage with the ip address.
b.Client sends the request for a webpage.
c.Server checks the table and responds with the appropriate ip address.
d.if not found send an error message to the client.

SERVER PART
*/

#include "table.h"

int main(int argc, char **argv){
    
    Record webpage_table[DOMAIN_LIMIT];
    for(int i = 0;i<DOMAIN_LIMIT;i++)
        initialize(&webpage_table[i]);

    if(argc > 1){
        printf("\n No arguments needed for server. ");
        exit(0);
    }

    //UDP connection addresses
    struct sockaddr_in server_address, client_address;
    //Buffer to handle messages
    char buffer[1024];
    
    //Parameters for communication
    char* webpage = (char*)calloc(100, sizeof(char));
    char* address = (char*)calloc(100, sizeof(char));

    //Socket fd: IPv4, UDP, IP
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0)
        perror("\n Error: Unable to create connection");
    
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;   //IP family
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);      //Any free address
    server_address.sin_port = htons(5555);    //Port number 5555

    //Bind socket and fd
    if(bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("\n Error: Binding error");
    }

    int len = sizeof(client_address);

    //Create records in table
    createRecord(webpage_table, "google.com", "13.14.168.192");
    createRecord(webpage_table, "ssn.edu.in", "77.88.99.100");
    createRecord(webpage_table, "youtube.com", "92.68.3.4");
    createRecord(webpage_table, "ssn.edu.in", "99.111.223.123");

    //Modify table at runtime
    char option = 'n';
    do{
        table(webpage_table);
        printf("\n Modify table? (y/n) ");scanf(" %c", &option);
        if(option == 'y' || option == 'Y'){
            printf("\n Enter webpage name: "); scanf(" %[^\n]", webpage);
            printf("\n Enter address: "); scanf(" %[^\n]", address);
            
            int result = createRecord(webpage_table, webpage, address);
            if(result){
                printf("\n Table modified successfully\n");
            }
        }
    }while(option == 'y'|| option == 'Y');

    printf("\nWebpage server set up complete. \n");

    char *response = (char*)calloc(ADDR_LIMIT*2, sizeof(char));
	while(1){
		bzero(&buffer, sizeof(buffer));
		recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&client_address, &len);
		
		strcpy(response, getAddress(webpage_table, buffer));
		sendto(sockfd, response, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
	}
	close(sockfd);
}