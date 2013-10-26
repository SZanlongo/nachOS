#include "pcb.h"

PCB::PCB(Thread *currentThread, Thread *forkedThread, int nextPID) {
    parent = currentThread;
	thread = forkedThread;
	pid = nextPID;
	
	childList = new List();
}