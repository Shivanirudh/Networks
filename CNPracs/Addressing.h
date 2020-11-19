#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>

//Return 1 for classful, 0 for classless addressing
int find_cat(char *addr){
    for(int i = 0; addr[i]; i++){
        if(addr[i] == '/')
            return 0;
    }
    return 1;
}

int checkIP(char *ip){

    char *addr = (char*)calloc(100, sizeof(char));
    strcpy(addr, ip);


    int cat = find_cat(addr);
    if(!cat){
        for(int i = 0; addr[i]; i++){
            if(addr[i] == '/'){
                addr[i] = '\0';
                break;
            }
        }
    }

    int count = 0;
    for(int i = 0; addr[i]; i++){
        if(addr[i] == '.')
            count++;
    }
    if(count != 3)
        return 0;

    char *token = strtok(addr, ".");
    while(token){
        int val = atoi(token);
        if(val < 0 || val > 255)
            return 0;
        token = strtok(NULL, ".");
    }
    return 1;
}

//Find class
char* find_class(char *addr){
    char *copy = (char*)calloc(100, sizeof(char));
    strcpy(copy, addr);

    char *class_name = (char*)calloc(100, sizeof(char));
    char *token = strtok(addr, ".");
    if(token){
        int val = atoi(token);
        if(val >= 0 && val <= 127)
            strcpy(class_name, "A");
        else if(val <= 191)
            strcpy(class_name, "B");
        else if(val <= 223)
            strcpy(class_name, "C");
        else if(val <= 239)
            strcpy(class_name, "D");
        else 
            strcpy(class_name, "E");
    }
    return class_name;
}

//Find default mask
char* find_def_mask(char *class_name, char *addr){
    char *mask = (char*)calloc(100, sizeof(char));
    if(!strcmp(class_name, "A"))
        strcpy(mask, "255.0.0.0");
    else if(!strcmp(class_name, "B"))
        strcpy(mask, "255.255.0.0");
    else if(!strcmp(class_name, "C"))
        strcpy(mask, "255.255.255.0");
    else{
        strcpy(mask, "\0");
    }
}

/*
________________________________________________________________________________________________________________________________
*/
int power(int num, int exp){
    int pdt = 1;
    while(exp--){
        pdt *= num;
    }
    return pdt;
}

void strrev(char* s){
    int len = strlen(s);
    int i = 0;
    int j = len-1;
    while(i<j){
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

int conv_to_dec(char *number){
    int num = 0;
    char *copy = (char*)calloc(100, sizeof(char));
    strcpy(copy, number);
    for(int i = 0; copy[i]; i++){
        if(copy[i] == '1')
            num += power(2, i);
    }
    return num;
}

char* conv_to_bin(int number){
    char *bin = (char*)calloc(100, sizeof(char));
    int n = number;
    int pos=0;
    while(n>0){
        bin[pos++] = ('0'+(n%2));
        n /= 2;
    }
    bin[pos] = '\0';
    strrev(bin);
    return bin;
}
/*
________________________________________________________________________________________________________________________________
*/

char* find_first(char *addr){
    char *copy = (char*)calloc(100, sizeof(char));
    strcpy(copy, addr);
    int host_bits = 0;

    char *token = strtok(copy, "/");
    while(token){
        host_bits = atoi(token);
        token = strtok(NULL, "/");
    }

    for(int i = 0; copy[i]; i++){
        if(copy[i] == '/'){
            copy[i] = '\0';
            break;
        }
    }
    int ip[4];
    int ctr = 0;
    token = strtok(copy, ".");
    while(token){
        ip[ctr++] = atoi(token);
        token = strtok(NULL, ".");
    }
    int q, r, num;
    q = host_bits/8;
    r = host_bits%8;
    for(int i = 0; i<4;i++){
        if(i == q){
            num = ip[i];
            num = num >> (8-r);
            num = num << (8-r);
            ip[i] = num;
        }
        else if(i>q)
            ip[i] = 0;
    }
    char *v = (char*)calloc(100, sizeof(char));
    char *first = (char*)calloc(100, sizeof(char));
    for(int i = 0;i < 4; i++){
        sprintf(v, "%d", ip[i]);
        strcat(first, v);strcat(first, ".");
    }
    return first;
}

char* find_last(char *addr){
    char *copy = (char*)calloc(100, sizeof(char));
    strcpy(copy, addr);
    int host_bits = 0;
    
    char *token = strtok(copy, "/");
    while(token){
        host_bits = atoi(token);
        token = strtok(NULL, "/");
    }
    for(int i = 0; copy[i]; i++){
        if(copy[i] == '/'){
            copy[i] = '\0';
            break;
        }
    }
    int ip[4];
    int ctr = 0;
    token = strtok(copy, ".");
    while(token){
        ip[ctr++] = atoi(token);
        token = strtok(NULL, ".");
    }
    int q, r, num;
    q = host_bits/8;
    r = host_bits%8;
    for(int i = 0; i<4;i++){
        if(i>q){
            ip[i] = 255;
        }
        else if(i == q){
            num = ip[i];
            num = num >> (8-r);
            num = num << (8-r);
            ip[i] = num + power(2,(8-r)) - 1;
        }
    }
    char *v = (char*)calloc(100, sizeof(char));
    char *last = (char*)calloc(100, sizeof(char));
    for(int i = 0;i < 4; i++){
        sprintf(v, "%d", ip[i]);
        strcat(last, v);strcat(last, ".");
    }
    
    return last;
}

char* find_hosts(char *addr){
    char *copy = (char*)calloc(100, sizeof(char));
    strcpy(copy, addr);
    int host_bits = 0;
    char *token = strtok(copy, "/");
    while(token){
        host_bits = atoi(token);
        token = strtok(NULL, "/");
    }
    
    for(int i = 0; copy[i]; i++){
        if(copy[i] == '/'){
            copy[i] = '\0';
            break;
        }
    }
    host_bits = 32 - host_bits;
    char *host_count = (char*)calloc(100, sizeof(char));
    int count = power(2, host_bits);
    count -= 2;
    sprintf(host_count, "%d", count);
    return host_count;
}