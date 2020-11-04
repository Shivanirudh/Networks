#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
    return bin;
}

int check_position(int number, int position){
    char *bin=(char*)calloc(100, sizeof(char));
    strcpy(bin, conv_to_bin(number));
    int len = strlen(bin);

    return (bin[len - position]==1)? 1 : 0;
}

int main(){
    char *input = (char*)calloc(100, sizeof(char));
    char *ecode = (char*)calloc(100, sizeof(char));
    char *ocode = (char*)calloc(100, sizeof(char));

    printf("\n Enter input data: ");scanf(" %[^\n]", input);
    while(!checkBinary(input)){
        printf("\nNot a binary stream. Please re-enter.\n");
        scanf(" %[^\n]", input);
    }

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
    for(int i = 0;i<code_len;i++){
        int ctr = 0;
        int ham_bit = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == power(2, j))
                ham_bit = 1;
        }
        ecode[i] = ham_bit? '0' : ip[ctr];
        ocode[i] = ham_bit? '0' : ip[ctr];
        ctr++;
    }

    printf("\nEcode: %s", ecode);
    printf("\nOcode: %s", ocode);

    //Hamming code
    for(int i = 0;i<code_len;i++){
        int ham_bit = 0;
        int pos = 0;
        for(int j = 0; j < code_len && !ham_bit; j++){
            if((i+1) == power(2, j))
                ham_bit = 1;
                pos += 1;
                break;
        }
        if(ham_bit){
            int ctr = 0;
            for(int j = 0;j<code_len;j++){
                int check_pos = check_position(j+1, pos);
                if(ecode[j] == 1){
                    ctr++;
                }
            }
            ecode[i] = ctr%2? 1:0;
            ocode[i] = ctr%2? 0:1;
        }
    }

    printf("\nInput data: %s", input);
    printf("\nEven parity code: %s", ecode);
    printf("\nOdd parity code: %s\n", ocode);
}