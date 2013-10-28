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

void *PCB::DeleteChild (int child) {
	return childList->SortedRemove(child);
}
