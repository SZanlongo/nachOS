#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "list.h"
#include "bitmap.h"

class ProcessManager {
    public:
		List *pcbList;
		ProcessManager();
		
		int GetID();
	
	private:
		BitMap *idMap;
	    int occupiedIDs;
};

#endif