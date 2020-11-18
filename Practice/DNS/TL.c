#include "Auth.c"

struct TLTable{
    Record table[DOMAIN_LIMIT];
};

typedef struct TLTable Top_level;
