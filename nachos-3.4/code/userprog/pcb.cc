#include "pcb.h"

PCB::PCB(Thread *thread, Thread *parent, int id) {
    procID = id;
	p = parent;
	t = thread;
	childList = new List();
}

int PCB::GetID() {
	return procID;
}