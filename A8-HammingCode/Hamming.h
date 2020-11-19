//Hamming code

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

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

int checkBinary(char *code){
    int check = 1;
    for(int i = 0; code[i] && check; i++){
        if(code[i] != '1' && code[i] != '0')
            check = 0;
    }
    return check;
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

int check_position(int number, int position){
    char *bin=(char*)calloc(100, sizeof(char));
    strcpy(bin, conv_to_bin(number));
    int len = strlen(bin);
    return (bin[len - position]=='1')? 1 : 0;
}

char* even_parity(char *input){
    char *ecode = (char*)calloc(100, sizeof(char));
    int ip_len = strlen(input);
    int red_bits = 0;
    for(int i = 0; i<100; i++){
        int lhs = power(2.0, i);
        int rhs = ip_len + i + 1;
        if( lhs >= rhs){
            red_bits = i;
            break;
        }
    }
    char *ip = (char*)calloc(100, sizeof(char));
    strcpy(ip, input);
    strrev(ip);
    int code_len = ip_len + red_bits;
    //Assign data bits
    int ip_ctr = 0;
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == power(2, j))
                ham_bit = 1;
        }
        if(ham_bit){
            ecode[i] = '0';
        }
        else{
            ecode[i] = ip[ip_ctr];;
            ip_ctr++;
        }
    }

    //Hamming code
    int pos = 0; //Position to check in binary value
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == power(2, j)){
                ham_bit = 1;
                pos += 1;
            }
        }
        if(ham_bit){
            int ctr = 0;
            for(int j = 0;j<code_len;j++){
                int check_pos = check_position(j+1, pos);
                if(ecode[j] == '1'&&check_pos){
                    ctr++;
                }
            }
            ecode[i] = ctr%2? '1':'0';
        }
    }
    //Reversing code
    strrev(ecode);
    return ecode;
} 

char *odd_parity(char *input){
    char *ocode = (char*)calloc(100, sizeof(char));

    int ip_len = strlen(input);
    int red_bits = 0;
    for(int i = 0; i<100; i++){
        int lhs = power(2.0, i);
        int rhs = ip_len + i + 1;
        if( lhs >= rhs){
            red_bits = i;
            break;
        }
    }
    char *ip = (char*)calloc(100, sizeof(char));
    strcpy(ip, input);
    strrev(ip);
    int code_len = ip_len + red_bits;
    //Assign data bits
    int ip_ctr = 0;
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == power(2, j))
                ham_bit = 1;
        }
        if(ham_bit){
            ocode[i] = '0';
        }
        else{
            ocode[i] = ip[ip_ctr];
            ip_ctr++;
        }
    }

    //Hamming code
    int pos = 0; //Position to check in binary value
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == power(2, j)){
                ham_bit = 1;
                pos += 1;
            }
        }
        if(ham_bit){
            int ctr = 0;
            for(int j = 0;j<code_len;j++){
                int check_pos = check_position(j+1, pos);
                if(ocode[j] == '1'&&check_pos){
                    ctr++;
                }
            }
            ocode[i] = ctr%2? '0':'1';
        }
    }

    //Reversing code
    strrev(ocode);
    return ocode;
}

int compute_error_pos(char *code, int parity){
    int code_len = strlen(code);
    char *value = (char*)calloc(100, sizeof(char));

    int red_bits = 0, ip_len = 0;
    for(int i = 0;i<code_len; i++){
        ip_len = code_len - i;
        int lhs = power(2, i);
        int rhs = ip_len + i + 1;
        if(lhs >= rhs){
            red_bits = i;
            break;
        }
    }
    for(int i =0; i<red_bits; i++){
        value[i] = '0';
    }
    int vctr = 0;
    int pos = 0;
    if(parity){
        for(int i = 0;i<code_len;i++){
            int ham_bit = 0;
            for(int j = 0; j < code_len && !ham_bit; j++){
                if((i+1) == power(2, j)){
                    ham_bit = 1;
                    pos += 1;
                }
            }
            if(ham_bit){
                int ctr = 0;
                for(int j = 0;j<code_len;j++){
                    int check_pos = check_position(j+1, pos);
                    if(code[j] == '1'&&check_pos){
                        ctr++;
                    }
                }
                value[vctr++] = ctr%2? '0':'1';
            }
        }
    }
    else{
        for(int i = 0;i<code_len;i++){
            int ham_bit = 0;
            for(int j = 0; j < code_len && !ham_bit; j++){
                if((i+1) == power(2, j)){
                    ham_bit = 1;
                    pos += 1;
                }
            }
            if(ham_bit){
                int ctr = 0;
                for(int j = 0;j<code_len;j++){
                    int check_pos = check_position(j+1, pos);
                    if(code[j] == '1'&&check_pos){
                        ctr++;
                    }
                }
                value[vctr++] = ctr%2? '1':'0';
            }
        }
    }
    strrev(value);
    return conv_to_dec(value);
}

char* decode(char *code){
    char *data = (char*)calloc(100, sizeof(char));

    int code_len = strlen(data);
    int red_bits = 0, ip_len = 0;
    for(int i = 0; i<100; i++){
        ip_len = code_len - i;
        int lhs = power(2.0, i);
        int rhs = ip_len + i + 1;
        if( lhs >= rhs){
            red_bits = i;
            break;
        }
    }

    char *copy = (char*)calloc(100, sizeof(char));
    strcpy(copy, code);
    //Hamming code
    int pos = 0; //Position to check in binary value
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == power(2, j)){
                ham_bit = 1;
                pos += 1;
            }
        }
        if(ham_bit){
            continue;
        }
        else{
            strcat(data, copy[i]);
        }
    }

    //Reversing code
    strrev(data);
    return data;
}

/*
Output:
 Enter input data: 1011001

Input data: 1011001
Even parity code: 10101001110
Odd parity code: 10111000101
*/
