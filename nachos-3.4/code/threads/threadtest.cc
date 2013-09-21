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

#include "copyright.h"
#include "system.h"

#if defined(HW1_SEMAPHORES) || defined(HW1_LOCKS) || defined(HW1_CONDITIONS)
#include "synch.h"
#endif

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

#if defined(CHANGED)
int SharedVariable;
int numWriters;

#if defined(HW1_SEMAPHORES)
Semaphore *semaphore;
#endif

#if defined(HW1_LOCKS) || defined(HW1_CONDITIONS)
Lock *lock;
#endif

#if defined(HW1_CONDITIONS)
Condition *condition;
#endif

void SimpleThread(int which) {
    int num, val;
    
    for(num = 0; num < 5; num++) {
#if defined(HW1_SEMAPHORES)
        semaphore->P();
#endif

#if defined(HW1_LOCKS)
        lock->Acquire();
#endif

#if defined(HW1_CONDITIONS)
        condition->Wait(lock);
#endif

        val = SharedVariable;
        printf("*** thread %d sees value %d\n", which, val);
        currentThread->Yield();
        SharedVariable = val+1;
        
#if defined(HW1_SEMAPHORES)
        semaphore->V();
#endif

#if defined(HW1_LOCKS)
        lock->Release();
#endif

#if defined(HW1_CONDITIONS)
        condition->Signal(lock);
#endif

        currentThread->Yield();
    }
    
    // this thread is no longer a writer
    numWriters--;
    printf("%s...numWriters=%d\n", currentThread->getName(), numWriters);
    
    // wait until all threads have finished updating the shared variable
    // (this is the "barrier")
    while (numWriters > 0) currentThread->Yield();
    DEBUG('t', "'%s' resuming after numWriters=0\n", currentThread->getName());
    
    val = SharedVariable;
    printf("Thread %d sees final value %d\n", which, val);
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

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------
#if defined(CHANGED) && defined(THREADS)
void ThreadTest(int n) {
    DEBUG('t', "ThreadTest(): called with n=%d\n", n);
    
    // each created thread is a writer, plus the main thread
    numWriters = n + 1;
    
#if defined(HW1_SEMAPHORES)
    semaphore = new Semaphore("testSemaphore", 1);
#endif

#if defined(HW1_LOCKS) || defined(HW1_CONDITIONS)
    lock = new Lock("testLock");
#endif

#if defined(HW1_CONDITIONS)
    condition = new Condition("testCondition");
#endif
    
    for (int i = 1; i <= n; i++) {
        char *threadName = new char[13];
        sprintf(threadName, "testThread-%d", i);
        threadName[12] = '\0';
        
        DEBUG('t', "Spawning thread '%s'\n", threadName);
        Thread *t = new Thread(threadName);
        
        t->Fork(SimpleThread, i);
    }
    
    SimpleThread(0);
    
    DEBUG('t', "Finished spawning threads, awaiting all of them to finish\n");
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
