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

#include "sysopenfile.h"
#include "useropenfile.h"

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

void GetName(char *fileName) {
	int pos = 0;
	int i;
	int arg = machine->ReadRegister(4);
	while (i != NULL) {
		machine->ReadMem(arg, 1, &i);
		fileName[pos++] = (char) i;
		arg++;
	}
}

char *copyChar(char *original) {
    char *newChar = new char[128];

    for (int i = 0; i < 128; i++) {
        newChar[i] = original[i];
        if (original[i] == NULL) {
            break;
        }
    }

    return newChar;
}

void myCreate(char *fileName) {
	printf("System Call: [%d] invoked Create\n", currentThread->space->pcb->pid);
	
	fileSystem->Create(fileName, 0);
}

OpenFileId myOpen(char *fileName) {
    printf("System Call: [%d] invoked Open\n", currentThread->space->pcb->pid);

    int index = 0;
    SysOpenFile *soFile = fileManager->Get(fileName, index);
//	int index = 0;
	if (soFile != NULL) {
		soFile->userOpens++;
	} else {
		OpenFile *oFile = fileSystem->Open(fileName);
		
		if (oFile == NULL) {
			return -1;
		}
		
		soFile = new SysOpenFile (fileName, 0, oFile);
		index = fileManager->Add(soFile);
	}
	
	UserOpenFile *uoFile = new UserOpenFile();
	uoFile->fileName = copyChar(fileName);
	uoFile->index = index;
	uoFile->offset = 0;
	
	return currentThread->space->pcb->Add(uoFile);
}

int myRead(int buffAdd, int size, OpenFileId ofID) {
	printf("System Call: [%d] invoked Read\n", currentThread->space->pcb->pid);
	
	char *buffer = new char[size + 1];
	int newSize = size;
	
	if (ofID == ConsoleInput) {
		for (int i = 0; i < size; i++) {
			buffer[i] = getchar();
		}
	} else {
		UserOpenFile *uoFile = currentThread->space->pcb->Get(ofID);
		
		if (uoFile == NULL) {
			return -1;
		} else {
			//should acquire lock here
			SysOpenFile *soFile = fileManager->Get(uoFile->index);
			newSize = soFile->openFile->ReadAt(buffer, size, uoFile->offset);
			//should release lock here			
		}
		
		ReadWrite (buffAdd, buffer, newSize, READ);
		delete[] buffer;
		
		return newSize;
	}
}

void myWrite(int buffAdd, int size, OpenFileId ofID) {
	printf("System Call: [%d] invoked Write\n", currentThread->space->pcb->pid);
	
	char *buffer = new char[size + 1];
	
	if (ofID == ConsoleOutput) {
		ReadWrite (buffAdd, buffer, size, WRITE);
		buffer[size] = 0;
		printf("%s", buffer);		
	} else {
		buffer = new char[size];
		int writeSize = ReadWrite(buffAdd, buffer, size, WRITE);
		UserOpenFile *uoFile = currentThread->space->pcb->Get(ofID);
		
		if (uoFile == NULL) {
			return;
		} else {
			SysOpenFile *soFile = fileManager->Get(uoFile->index);
			int bytes = soFile->openFile->WriteAt(buffer, size, uoFile->offset);
			uoFile->offset += bytes;
		}
	}
	delete[] buffer;
}

void myClose(OpenFileId ofID) {
	printf("System Call: [%d] invoked Close\n", currentThread->space->pcb->pid);
	
	UserOpenFile *uoFile = currentThread->space->pcb->Get(ofID);
	if (uoFile == NULL) {
		return;
	} else {
		SysOpenFile *soFile = fileManager->Get(uoFile->index);
		currentThread->space->pcb->Remove(ofID);
	}
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    char *fileName = new char[128];
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
			
			case SC_Open: {                                
                GetName(fileName);
				OpenFileId index = myOpen(fileName);
				if (index != -1) {
					machine->WriteRegister(2, index);
				} else {
					printf("Can't open file");
				}
				
				break;
			}
			
			case SC_Create: {	
				GetName(fileName);
				myCreate(fileName);
				
				break;
			}
			
			case SC_Read: {
				int arg1 = machine->ReadRegister(4);
				int arg2 = machine->ReadRegister(5);
				int arg3 = machine->ReadRegister(6);
				machine->WriteRegister(2, myRead(arg1, arg2, arg3));
				
				break;
			}
			
			case SC_Write: {
				int arg1 = machine->ReadRegister(4);
				int arg2 = machine->ReadRegister(5);
				int arg3 = machine->ReadRegister(6);
				myWrite(arg1, arg2, arg3);
				break;
			}
			
			case SC_Close: {
				myClose(machine->ReadRegister(4));
				break;
			}

		}
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}
