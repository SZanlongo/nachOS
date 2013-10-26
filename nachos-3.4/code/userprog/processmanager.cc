//process manager
//also known as pcb manager

#include "processmanager.h"

ProcessManager *ProcessManager():idMap(NumPhysPages) {
    pcbList = new List();
	pmLock = new Lock("process manager lock");
}

int ProcessManager::GetID() {
	pmLock->Aquire();
		int procID = idMap->Find();
		if (procID >= 0) {
			occupiedIDs++;
		}
	pmLock->Release();
	
	return procID;
}