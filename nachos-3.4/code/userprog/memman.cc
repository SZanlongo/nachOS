//Memory Manager
//uses BitMap

#include "memman.h"
#include "machine.h"

memMan *memMan::man = NULL;

//if no existing Memory Manager, create; else return
memMan *memMan::getMan() {
    if (!man) {
        man = new memMan();
    }
    return memMan;
}

//create new Memory Manager
memMan::memMan(int pages) {
    pages = new BitMap(pages);
    memLock = new Lock("memLock");
    total = pages;
    occupied =0;
    i = 0;
}

//Allocates the first clear page
int memMan::getPage() {
    if (used >= total) {
        return -1;
    }

    //Find next free page
    memLock -> Acquire();
    int pageNum = pages -> Find();
    if (pageNum >= 0) {
        occupied++;
    }
    memLock -> Release();

    return pageNum;
}

//Takes the index of a page and frees it
bool memMan::clearPage(int i) {
    
}
