#define MAX_ADDR 10
#define MAX_DOMAIN 20
typedef char string[30];

typedef struct Entry
{
    string domain;
    string address[MAX_ADDR];
} Entry;

void printTable(Entry table[MAX_DOMAIN])
{
    printf("+-----------------+----------------------+\n");
    printf("|   Domain Name   |        Address       |\n");
    printf("+-----------------+----------------------+\n");
    for (int i = 0; i < MAX_DOMAIN; i++)
    {
        if (table[i].domain[0])
        {
            printf("| %-15s | %-20s |\n", table[i].domain, table[i].address[0]);

            for (int j = 1; j < MAX_ADDR && table[i].address[j][0]; j++)
                printf("| %-15s | %-20s |\n", "", table[i].address[j]);
            printf("+-----------------+----------------------+\n");
        }
    }
    printf("\n");
}

int checkAddress(Entry *table, char *const address)
{

    string addr_copy;
    strcpy(addr_copy, address);
    char *split;
    int val;
    split = strtok(addr_copy, ".");
    while (split)
    {
        val = atoi(split);
        if (val < 0 || val > 255)
        {
            printf("Error: Invalid Address!\n");
            return 0;
        }
        split = strtok(NULL, ".");
    }

    for (int i = 0; i < MAX_DOMAIN; i++)
    {
        if (!table[i].domain[0])
            continue;

        for (int j = 0; j < MAX_ADDR && table[i].address[j][0]; j++)
            if (strcmp(address, table[i].address[j]) == 0)
            {
                printf("Error: Duplicate IP address!\n");
                return 0;
            }
    }

    return 1;
}

int createEntry(Entry table[MAX_DOMAIN], char *domain, char *address)
{
    // Search if entry exists already
    int index = -1;
    int flag = 0;

    int addr_valid = checkAddress(table, address);

    if (!addr_valid)
        return flag;

    for (int i = 0; i < MAX_DOMAIN; i++)
    {
        if (strcmp(table[i].domain, domain) == 0)
        {
            for (int j = 0; j < MAX_DOMAIN; j++)
                if (!table[i].address[j][0])
                {
                    strcpy(table[i].address[j], address);
                    flag = 1;
                    break;
                }
            break;
        }
        if (!table[i].domain[0] && index == -1)
            index = i;
    }

    // IF entry has to be created
    if (!flag)
    {
        strcpy(table[index].domain, domain);
        strcpy(table[index].address[0], address);
        flag = 1;
    }

    return flag;
}

Entry *getAddress(Entry *table, char *const domain)
{
    static Entry result;
    bzero(&result, sizeof(Entry));
    strcpy(result.domain, domain);

    for (int i = 0; i < MAX_DOMAIN; i++)
    {

        if (strcmp(table[i].domain, domain) == 0)
        {
            for (int j = 0; j < MAX_ADDR; j++)
            {
                strcpy(result.address[j], table[i].address[j]);
            }
            break;
        }
    }
    return (&result);
}
