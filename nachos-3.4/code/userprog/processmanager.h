#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "list.h"
#include "bitmap.h"
#include "synch.h"

class ProcessManager {
    public:
		List *pcbList;
		ProcessManager();
		void *ProcessManager::DeletePID(int pid);
		//void *ProcessManager::Remove(int pcb);
		int NextPID();
	
	private:
		int PIDsUsed;
		BitMap *PIDList;
		Lock *pmLock;
};

#endif
