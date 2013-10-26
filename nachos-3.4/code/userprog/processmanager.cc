//process manager
//also known as pcb manager

#include "processmanager.h"

ProcessManager::ProcessManager() {
    pcbList = new List();
	pmLock = new Lock("process manager lock");
	idMap = new BitMap(512);
}

int ProcessManager::GetID() {
	pmLock->Acquire();
		int procID = idMap->Find();
		if (procID >= 0) {
			occupiedIDs++;
		}
	pmLock->Release();
	
	return procID;
}
