#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "bitmap.h"
#include "list.h"

class Thread;

class PCB {
	public:
		PCB(Thread *thread, Thread *parent, int id);
		
		List *childList;
	
		int GetID();
	
	private:
		int procID;
		Thread *t;
		Thread *p;
		
};

#endif