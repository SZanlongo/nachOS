#include "memorymanager.h"
#include "machine.h"

MemoryManager::MemoryManager() {
    pages = new BitMap(512);
	mmLock = new Lock("memory manager lock");
	pagesUsed = 0;
	totalPages = 512;
}
