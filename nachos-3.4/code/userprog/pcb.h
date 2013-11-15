class Thread;
class Lock;

#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "bitmap.h"
#include "list.h"

#include "useropenfile.h"

class PCB {
	public:
		PCB(Thread *currentThread, Thread *forkedThread, int nextPID);
		
		Thread *parent;
		Thread *thread;
		int pid;
		int exit;
		
		List *childList;
		void *FindChild (int child);
		
		UserOpenFile *openFiles[128];
		int Add(UserOpenFile *uoFile);
		bool Remove(int fileID);
		UserOpenFile *Get(int fileID);
		BitMap *fileMap;
};

#endif
