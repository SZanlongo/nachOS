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
void 
myFork(int addressSpace);

void 
myYield();

void 
myExec(int addressSpace);

void 
myJoin(int addressSpace);

void
myExit(int addressSpace);

void 
ExecuteSystemCall(int typeOfSysCall);

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

// old code
//    if ((which == SyscallException) && (type == SC_Halt)) {
//	DEBUG('a', "Shutdown, initiated by user program.\n");
//   	interrupt->Halt();
//    } else {
//	printf("Unexpected user mode exception %d %d\n", which, type);
//	ASSERT(FALSE);
//    }
    if(which == SyscallException){
        ExecuteSystemCall(type);
    }
    else {
    	printf("Unexpected user mode exception %d %d\n", which, type);
    	ASSERT(FALSE);
    }
}

void ExecuteSystemCall(int typeOfSysCall){

    int result;
    switch(typeOfSysCall){

     case SC_Halt:
         interrupt->Halt();
         break;
     
     case SC_Fork:
         myFork(machine->ReadRegister(4));
//         interrupt->Halt();
         break;

     case SC_Yield:
         myYield();
//         interrupt->Halt();
         break;
     
     case SC_Exec:
         myExec(machine->ReadRegister(4));
//         interrupt->Halt();
         break;

     case SC_Join:
         myJoin(machine->ReadRegister(4));
//         interrupt->Halt();
         break;

     case SC_Exit:
         myExit(machine->ReadRegister(4));
         interrupt->Halt();
         break;
    }
}

void 
myFork(int addressSpace){

    printf("Forking.\n");
	DEBUG('a', "My fork test.\n");
}

void
myYield(){

    printf("Yielding.\n");
	DEBUG('a', "My yield test.\n");
}

void
myExec(int addressSpace){

    printf("Executing.\n");
	DEBUG('a', "My executing test.\n");
}

void
myJoin(int addressSpace){ 

    printf("Joining.\n");
	DEBUG('a', "My join test.\n");
}

void
myExit(int addressSpace){

    printf("Exiting.\n");
	DEBUG('a', "My exiting test.\n");
}
