// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#include "addrspace.h"
#include "filesys.h"

#include "pcb.h"
#include "processmanager.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void myFork(int);
void Dummy(int);

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

	/*
	Which system call is called? Whenever a system call is invoked, print: 
	System Call: [pid] invoked [call]  
	where [pid] is the identifier of the process (SpaceID) and [call] is the name of one of the system 
	calls that you had to implement. Just give the name without parentheses (e.g., Fork, Create, 
	Exit). 
	*/
	
	if(which == SyscallException) {
	    switch(type) {
		    case SC_Halt: {
				DEBUG('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt();
				break;
			}
			
			case SC_Fork: {
				myFork(machine->ReadRegister(4));				
			    break;
			}
			
			case SC_Yield: {
			    break;
			}
			
			case SC_Exec: {
				break;
			}
			
			case SC_Join: {
				
				break;
			}
			
			case SC_Exit: {
				break;
			}
		}
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}

void Dummy(int tempState) {	
	int* state = (int *) tempState;
        for(int i = 0; i < NumTotalRegs; i++) {
            machine->WriteRegister(i, state[i]);
		}
        currentThread->space->RestoreState();
        machine->Run();
}

void myFork(int vSpace) {
		printf("System Call: [%d] invoked Fork\n", currentThread->space->pcb->pid);
		
		ProcessManager* pm = new ProcessManager();
		//save old registers
		currentThread->space->SaveState();

		//find next pid
		int nextPID = pm->NextPID();
		//duplicate address space
		AddrSpace* forkedSpace = currentThread->space->Duplicate(nextPID);

		Thread* forkedThread = new Thread("forked thread");
	
		//copy old register values into new thread;
		int* oldRegisters = new int[NumTotalRegs];
        for(int i =0; i < NumTotalRegs;i++) {
            oldRegisters[i] = machine->ReadRegister(i);
		}
        oldRegisters[PCReg] = vSpace;
        oldRegisters[NextPCReg] = vSpace+4;

		PCB* pcb = new PCB(currentThread, forkedThread, nextPID);
	       
		//add new thread pcb to child list of current thread
		currentThread->space->pcb->childList->SortedInsert((void*)pcb, pcb->pid);

		forkedSpace->pcb = pcb;
		//insert into pcb list (by pid)
		pm->pcbList->SortedInsert((void*)pcb, pcb->pid);

		printf("Process [%d] Fork: start at address [0x%x] with [%d] pages memory\n", currentThread->space->pcb->pid, vSpace, forkedSpace->GetNumberPages());
		forkedThread->space = forkedSpace;

		forkedThread->Fork(Dummy, vSpace);
		
		machine->WriteRegister(2, nextPID);
		currentThread->Yield();
}
