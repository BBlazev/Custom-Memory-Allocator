#include <stdio.h>
#include "alloc.h"

extern void* memspace;

header *findBlock_(header *hdr,word allocation, word n){

    bool ok;
    void *mem;
    header *hdr_;
    word n_;


    if ((n+ allocation) > (Maxwords -2))
        returnError(ErrorNoMemory);

    ok = false;
    ok =    (!(hdr->w)) ? true : 
            (!(hdr->alloced)) && (hdr->w >= allocation) ? true : false;

    if(ok) return  hdr;

    else {
        mem = (void *) hdr + hdr->w;
        hdr_ = (header*) mem;
        n_ = n + hdr->w;

        return findBlock_(hdr_, allocation, n_);
    }

    returnError(ErrorUnknown);


}



void* make_allocation(word words, header *hdr){
    void *ret;
    size_t bytes_in;  
    word words_in;
    
    bytes_in = ((char*)hdr - (char*)memspace);
    words_in = (((word)bytes_in)/4)+1;
    
    if(words > (Maxwords - words_in))
        returnError(ErrorNoMemory);
    
    hdr->w = words;
    hdr->alloced = true;
    ret = ((void*) hdr) + 4;

    return ret;
}

void *alloc(int32 bytes){
    word words;
    header * hdr;
    void *mem, *ret; 
    
    words = (!(bytes % 4)) ? bytes / 4 : (bytes / 4) + 1;

    mem = (void*) memspace;
    hdr = (header*) mem;

    if (!(hdr->w)) {
        if(words > Maxwords)
            returnError(ErrorNoMemory);
        
        
        if(!(ret = make_allocation(words, hdr)))
            return (void*)0;
        
        return ret;  
    } 
    else {
        printf("works\n");
        exit(0);
    }
}

int main(int argc, char* argv[]) {
     
    header *p;
    int8 *x;
    x = alloc(5);
    printf("0x%x\n", (int*) x);

    p = findBlock(500);
    if(!p){
        printf("Error %d\n", errno);
        return -1;
    }
    printf("0x%x\n", (int*) memspace);
    printf("0x%x\n", (int*) p);

    return 0;
}