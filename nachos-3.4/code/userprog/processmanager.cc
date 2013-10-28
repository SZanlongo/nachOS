//process manager
//also known as pcb manager

#include "processmanager.h"

ProcessManager::ProcessManager() {
    pcbList = new List();
	PIDList = new BitMap(512);
	//0 is reserved
	PIDList->Mark(0);
	PIDsUsed = 0;
	pmLock = new Lock("process manager lock");
}

int ProcessManager::NextPID() {
    if (PIDsUsed >= 512) {
	    return -1;
	} else {
		pmLock->Acquire();
			int pid = PIDList->Find();
			
			if (pid >= 0) {
			    PIDsUsed++;
			}
		pmLock->Release();

		return pid;
	}
}

void *ProcessManager::Remove(int pcb) {
	return pcbList->Remove(pcb);
}

void *ProcessManager::DeletePID(int pid) {
	if (PIDsUsed > 0) {
		PIDList->Clear(pid);
		PIDsUsed--;
	}
}
