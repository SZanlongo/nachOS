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
#include "filemanager.h"

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

char *clone(char *original) {
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
	
	int path = machine->ReadRegister(4);
	
	int hold;
	int i = 0;
	
	while (hold != NULL) {
		machine->ReadMem(path, 1, &hold);
		fileName[i++] = (char) hold++;
	}
	
	fileSystem->Create(fileName, 0);
}

OpenFileId myOpen(char *fileName) {
    printf("System Call: [%d] invoked Open\n", currentThread->space->pcb->pid);

    int index = 0;
    SysOpenFile *sysoFile = FileManager->Get(fileName, index);
    
    if (sysoFile == NULL) {
        OpenFile *openFile = fileSystem->Open(fileName);
        SysOpenFile *sysFile;
        sysFile->openFile = openFile;
        sysFile->nUsers = 1;
        sysFile->fileName = clone(fileName);
        index = FileManager->Add(sysFile);
    } else {
        sysoFile->nUsers++;
    }

    UserOpenFile useroFile();
    useroFile->index = index;
    useroFile->offset = 0;
    OpenFileId openFileID = currentThread->space->pcb->Add(useroFile);
    return openFileID;
}

void myRead() {
	printf("System Call: [%d] invoked Read\n", currentThread->space->pcb->pid);
}

void myWrite() {
	printf("System Call: [%d] invoked Write\n", currentThread->space->pcb->pid);
}

void myClose() {
	printf("System Call: [%d] invoked Close\n", currentThread->space->pcb->pid);
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
			} case SC_Open: {
                                
                                clone(fileName);
				int ans = myOpen(fileName);
                                machine->WriteRegister(2, ans);
				break;
			}case SC_Create: {
				myCreate(fileName);
				break;
			}case SC_Read: {
				myRead();
				break;
			}case SC_Write: {
				myWrite();
				break;
			}case SC_Close: {
				myClose();
				break;
			}

		}
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}
