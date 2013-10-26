#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "addrspace.h"
#include "synch.h"
#include "bitmap.h"

class AddrSpace;
class Lock;

class MemoryManager {
	public:
		MemoryManager();
		int totalPages;
	private:
		BitMap *pages;
		Lock *mmLock;
		int pagesUsed;
};

#endif
