#include "pcb.h"

PCB::PCB(Thread *currentThread, Thread *forkedThread, int nextPID) {
    parent = currentThread;
	thread = forkedThread;
	pid = nextPID;
	
	childList = new List();
}

void *PCB::FindChild(int child) {
	ListElement *element = NULL;	
    for (element = childList->GetFirst(); element != NULL; element = element->next) {
		if (element->key == child) {
			return element->item;
		}
	}
	return NULL;
}

int PCB::Add(UserOpenFile *uoFile) {
	int index = fileMap->Find();
	
	if (index > -1) {
		openFiles[index] = uoFile;
	}
	
	return index;
}

UserOpenFile *PCB::Get(int id) {
	if (fileMap->Test(id)) {
		return openFiles[id];
	} else {
		return NULL;
	}
}

bool PCB::Remove(int id) {
	if (fileMap->Test(id)) {
		delete openFiles[id];
		fileMap->Clear(id);
		return true;
	} else {
		return false;
	}
}
