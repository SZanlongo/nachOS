#include "memorymanager.h"
#include "machine.h"

MemoryManager::MemoryManager() {
    pages = new BitMap(512);
}
    