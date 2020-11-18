#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(){
	FILE *fp;
	char op[5024];
	char buffer[100];
	fp = popen("ls", "r");
	int read, i=0;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    	// printf("%s", path);
		strcat(op, buffer);
  	}
	printf("READ: %s\n", op);
	fclose(fp);
}