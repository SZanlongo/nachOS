#ifndef MEMMAN_H
#define MEMMAN_H

#include "synch.h"
#include "bitmap.h"
#include "addrspace.h"

class MemMan {
    public:
        int i, total;
        static MemMan *getMan();
        int getPage();
        bool clearPage(int i); 
    private:
        BitMap *pages;
        int used;
        Lock *memLock;        
        MemMan(int pages);
        static MemMan *man;
};

#endif
