#include "memorymanager.h"
#include "machine.h"

MemoryManager *MemoryManager::mm = NULL;

MemoryManager::MemoryManager():pages(NumPhysPages) {
}
    