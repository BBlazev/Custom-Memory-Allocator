#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>

#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))

#define Maxwords ((1024*1024*1024/4)-1)

#define ErrorNoMemory   1
#define ErrorUnknown    2
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;
typedef unsigned _BitInt(128) int128;
typedef void heap;
typedef int32 word;

struct packed s_header{

    word w: 30;
    bool alloced: 1;
    bool unused reserved: 1;
};

typedef struct packed s_header header;


#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $16 (int128)
#define $c (char*)
#define $i (int)
#define $v (void*)
#define $h (header*)

#define returnError(x) do{ \
    errno = (x);\
    return (void*) 0;\
} while(false)





#define findBlock(x) findBlock_((header*)memspace,(x),0)
header *findBlock_(header*, word, word);

void *make_allocation(word,header*);
void *alloc(int32 bytes);

int main(int, char**);


