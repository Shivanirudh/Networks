#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define ADDR_LIMIT 5
#define DOMAIN_LIMIT 10

//Record of each domain-address pair
struct record{
	char *domain;
	char *address[ADDR_LIMIT];
};

typedef struct record Record;

void init(Record *r){
	r->domain = (char*)calloc(100, sizeof(char));
	for(int i =0;i<ADDR_LIMIT;i++)
		r->address[i] = (char*)calloc(100, sizeof(char));
}

//Print DNS Table
void DNSTable(Record table[DOMAIN_LIMIT]){
    printf(" ________________________________________ \n");
    printf("|___Domain Name___|________Address_______|\n");
    
    for (int i = 0; i < DOMAIN_LIMIT; i++){
        if (table[i].domain[0]){
            printf("| %-15s | %-20s |\n", table[i].domain, table[i].address[0]);

            for (int j = 1; j < ADDR_LIMIT && table[i].address[j][0]; j++)
                printf("| %-15s | %-20s |\n", "", table[i].address[j]);
            printf("|_________________|______________________|\n");
        }
    }
    printf("\n");
}

//Check if newly specified address is a valid address
int checkAddress(Record *table, char *address){
    
    char* addr_copy = (char*)calloc(100, sizeof(char));
    strcpy(addr_copy, address);
    char *split;
    int val;
    split = strtok(addr_copy, ".");
    //Check if all octets lue within 0 and 255.
    while (split){
        val = atoi(split);
        if (val < 0 || val > 255){
            printf("\nError: Invalid Address.\n");
            return 0;
        }
        split = strtok(NULL, ".");
    }

    //Check if new address already exists in the table
    for (int i = 0; i < DOMAIN_LIMIT; i++){
        if (!table[i].domain[0])
            continue;

        for (int j = 0; j < ADDR_LIMIT && table[i].address[j][0]; j++)
            if (strcmp(address, table[i].address[j]) == 0){
                printf("\nError: IP address already exists.\n");
                return 0;
            }
    }

    return 1;
}

//Create DNS-address pair in the table
int createRecord(Record table[DOMAIN_LIMIT], char *domain, char *address){
    	
    int ix = -1;
    int flag = 0;
	//Check if entry exists already
    int addr_valid = checkAddress(table, address);
    if (!addr_valid)
        return flag;

    for (int i = 0; i < DOMAIN_LIMIT; i++){
        if (strcmp(table[i].domain, domain) == 0){
            for (int j = 0; j < DOMAIN_LIMIT; j++)
                if (!table[i].address[j][0]){
                    strcpy(table[i].address[j], address);
                    flag = 1;
                    break;
                }
            break;
        }
        if (!table[i].domain[0] && ix == -1)
            ix = i;
    }

    // If record can be created
    if (!flag){
        strcpy(table[ix].domain, domain);
        strcpy(table[ix].address[0], address);
        flag = 1;
    }

    return flag;
}

Record getAddress(Record *table, char *domain){
    
    Record result;
    init(&result);
    strcpy(result.domain, domain);
    printf("\n%s %s\n", domain, result.domain);
    for (int i = 0; i < DOMAIN_LIMIT; i++){
        if (strcmp(table[i].domain, domain) == 0){
            for (int j = 0; j < ADDR_LIMIT; j++)            {
                strcpy(result.address[j], table[i].address[j]);
            }
            break;
        }
    }
    printf("\nDomain: %s\nAddresses: ", result.domain);
    for(int i = 0;i<ADDR_LIMIT;i++)
        printf("%s ", result.address[i]);
    printf("\n");
    return result;
}
