// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#define HW1_LOCKS
#include "copyright.h"
#include "system.h"
#include "synch.h"
// testnum is set in main.cc
int testnum = 1;
Lock* threadLock;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

int SharedVariable; 
int amountOfForkedThreads;
#ifdef HW1_SEMAPHORES
Semaphore * semaphore;

void SimpleThread(int which) { 
    int num, val; 

    if(semaphore == NULL)
    {
        semaphore = new Semaphore("Test", 1);
    }

    for(num = 0; num < 5; num++) { 

        semaphore->P();
        val = SharedVariable;
        printf("*** thread %d sees value %d\n", which, val); 
        currentThread->Yield();
        SharedVariable = val + 1;
        semaphore->V();
        currentThread->Yield(); 
    } 

    amountOfForkedThreads--;
    while(amountOfForkedThreads)
    {
        currentThread->Yield();
    }

    val = SharedVariable; 
    printf("Thread %d sees final value %d\n", which, val); 
}
#elif defined(HW1_LOCKS)
// Spend some time possibly adding all these variables to a struct.
Lock* amountNotReadyForFinalOutputLock = new Lock("Amount not ready for final output lock.");

Condition* readyForFinalOutputCondition = new Condition("Ready for final output condition.");

Lock* numberOfForkedThreadsDoneLock = new Lock("Number of forked threads done lock.");

Condition* allForkedThreadsDoneCondition = new Condition("All forked threads done condition.");

int amountNotReadyForFinalOutput;

int numberOfForkedThreadsDone = 0;

void SimpleThread(int which){

    int num;

    if(threadLock == NULL)
    {
        threadLock = new Lock("ThreadTestLock");
    }

    for(num = 0; num < 5; num++) { 
        threadLock->Acquire();
        printf("*** thread %d sees value %d\n", which, SharedVariable); 
        SharedVariable += 1;
        threadLock->Release();
        currentThread->Yield(); 
    } 

    amountNotReadyForFinalOutputLock->Acquire();

    amountNotReadyForFinalOutput--;
    while(amountNotReadyForFinalOutput > 0) {
        readyForFinalOutputCondition->Wait(amountNotReadyForFinalOutputLock);
    }

    readyForFinalOutputCondition->Broadcast(amountNotReadyForFinalOutputLock);
    amountNotReadyForFinalOutputLock->Release();

    printf("Thread %d sees final value %d\n", which, SharedVariable); 

    numberOfForkedThreadsDoneLock->Acquire();
    numberOfForkedThreadsDone++;

    if (numberOfForkedThreadsDone == amountOfForkedThreads) {
        allForkedThreadsDoneCondition->Signal(numberOfForkedThreadsDoneLock);
    }

    numberOfForkedThreadsDoneLock->Release();
}

#else
void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}
#endif

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------
#ifdef HW1_SEMAPHORES
void
ThreadTest1(int numberOfThreads)
{
    DEBUG('t', "Entering ThreadTest1");

    int num;
    amountOfForkedThreads = numberOfThreads + 1;

    for(num = 0; num < numberOfThreads; num++)
    {
        Thread *t = new Thread("forked thread");

        t->Fork(SimpleThread, num + 1);
    }
    if(numberOfThreads >= 0)
    {
        SimpleThread(0);
    }
}
#elif defined(HW1_LOCKS)
void
ThreadTest1(int numberOfThreads)
{
    DEBUG('t', "Entering ThreadTest1");

    int num;
    amountOfForkedThreads = amountNotReadyForFinalOutput = numberOfThreads;

    for(num = 0; num < numberOfThreads; num++)
    {
        Thread *t = new Thread("forked thread 1");

        t->Fork(SimpleThread, num + 1);
    }

    numberOfForkedThreadsDoneLock->Acquire();
    
    while(numberOfForkedThreadsDone < amountOfForkedThreads) {
        allForkedThreadsDoneCondition->Wait(numberOfForkedThreadsDoneLock);
    }

    numberOfForkedThreadsDoneLock->Release();
}
#else
void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}
#endif
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

#ifdef HW1_SEMAPHORES
void
ThreadTest(int numberOfThreads)
{
    switch (testnum) {
    case 1:
	ThreadTest1(numberOfThreads);
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}
#elif defined(HW1_LOCKS)
void
ThreadTest(int numberOfThreads)
{
   switch (testnum) {
    case 1:
	ThreadTest1(numberOfThreads);
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}
#else
void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}
#endif

