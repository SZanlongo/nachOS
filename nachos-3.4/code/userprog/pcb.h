class Thread;
class Lock;

#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "bitmap.h"
#include "list.h"

class PCB {
	public:
		PCB(Thread *currentThread, Thread *forkedThread, int nextPID);
		
		Thread *parent;
		Thread *thread;
		int pid;
		
		List *childList;		
};

#endif
