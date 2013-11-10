// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

Lock::Lock(char* debugName) {
    name = debugName;
    currentLockHolder = NULL;
    isAvailable = true;
    waitingForLockQueue = new List();
}

Lock::~Lock() {
    delete waitingForLockQueue;
}

void Lock::Acquire() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    if(isAvailable) {
        isAvailable = false; 
        currentLockHolder = currentThread;  // remember who acquired it
    }
    else {
        waitingForLockQueue->Append((void*)currentThread);
        currentThread->Sleep();
    }
    
    interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

bool Lock::isHeldByCurrentThread() {
    return (currentLockHolder == currentThread);
}

void Lock::Release() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    if (this->isHeldByCurrentThread()) { // only the owner of the lock can release it

        if (waitingForLockQueue->IsEmpty()){
            isAvailable = true;
            currentLockHolder = NULL;
        }
        else {
            currentLockHolder = (Thread*)waitingForLockQueue->Remove();
            scheduler->ReadyToRun(currentLockHolder);
        }
    }
    
    interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
//#else
//// Dummy functions -- so we can compile our later assignments 
//// Note -- without a correct implementation of Condition::Wait(), 
//// the test case in the network assignment won't work!
//Lock::Lock(char* debugName) {}
//Lock::~Lock() {}
//void Lock::Acquire() {}
//void Lock::Release() {}
//#endif

//#if defined(HW1_CONDITIONS)
Condition::Condition(char* debugName) {
    name = debugName;
    waitingForConditionQueue = new List;
}

Condition::~Condition() {
    delete waitingForConditionQueue;
}

void Condition::Wait(Lock* conditionLock) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    DEBUG('t', "\tCondition::Wait(): thread '%s' entered, releasing lock\n", currentThread->getName());
    
    // first, release the lock (if it is being held by the current thread)
    conditionLock->Release();
    
    DEBUG('t', "\tCondition::Wait(): lock released, appending '%s' to wait queue\n", currentThread->getName());
    
    // thread waits to be signaled and gives up the CPU
    waitingForConditionQueue->Append((void*)currentThread);
    currentThread->Sleep();
    
    // resume once signaled and re-acquire the lock
    DEBUG('t', "\tCondition::Wait(): '%s' acquiring lock...\n", currentThread->getName());
    conditionLock->Acquire();
    DEBUG('t', "\tCondition::Wait(): '%s' acquired lock!\n", currentThread->getName());
    
    interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

void Condition::Signal(Lock* conditionLock) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    // TODO: While the first condition in line 195 ensures that the current thread holds 
    // TODO: the lock that is passed in it does not ensure that the locked that is 
    // TODO: passed in is the same one that the sleeping thread is waiting on.

    // check if there is a thread to signal
    if (conditionLock->isHeldByCurrentThread() && !waitingForConditionQueue->IsEmpty()) {
        Thread *wakeUpThread = (Thread*) waitingForConditionQueue->Remove();
        scheduler->ReadyToRun(wakeUpThread);
    } 
    
    interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

void Condition::Broadcast(Lock* conditionLock) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    // TODO: While the first condition in line 212  ensures that the current thread holds 
    // TODO: the lock that is passed in it does not ensure that the locked that is 
    // TODO: passed in is the same one that the sleeping thread is waiting on.

    // implement a broadcast by signaling EVERY waiting thread
    // note that a side effect of Signal() is that a thread is popped from the list
    if (conditionLock->isHeldByCurrentThread()){
        while (!waitingForConditionQueue->IsEmpty()) {
            Thread *wakeUpThread = (Thread*) waitingForConditionQueue->Remove();
            scheduler->ReadyToRun(wakeUpThread);
        }
    }
    
    interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
//#else
//Condition::Condition(char* debugName) { }
//Condition::~Condition() { }
//void Condition::Wait(Lock* conditionLock) { }
//void Condition::Signal(Lock* conditionLock) { }
//void Condition::Broadcast(Lock* conditionLock) { }
//#endif
