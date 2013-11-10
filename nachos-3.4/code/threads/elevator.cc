#include "elevator.h"

ElevatorSynchronization elevatorSynchronization = { new Condition("Arrived to new Floor condition."),
                                 new Condition("Attempt to enter elevator condition."),
                                 new Condition("Elevator users entered elevator condition."),
                                 new Condition("Elevator is active condition."),
                                 new Lock("Lock for elevator request."), 
                                 new Lock("Lock for entering elevator."),
                                 new Lock("Lock for closing elevator door."),
                                 new Lock("Lock for current capacity."),
                                 new Lock("Lock for the current floor.")};

// Creates an instance of an Elevator object with the specified number of floors.
SimpleElevator::SimpleElevator(int amountOfFloors) {

    numberOfFloors = amountOfFloors;
    currentCapacity = 5;
    currentFloor = 1;
    elevatorDirection = UP;
    int arr [numberOfFloors];
    requestPerFloor = arr;
    numberOfElevatorRequest = 0;
}

// Releases resources of an Elevator object.
SimpleElevator::~SimpleElevator(){
}


// Opens the doors of the elevator to allow current passangers to get out and new one to get in.
void SimpleElevator::OpenAndCloseDoors(){

    elevatorSynchronization.currentFloorLock->Acquire();
    elevatorSynchronization.arrivedToNewFloor->Broadcast(elevatorSynchronization.currentFloorLock); // Wakes up elevator users in the elevator so they can leave if 
    elevatorSynchronization.currentFloorLock->Release();                                            // elevator arrived to their requested floor.
                                                         

//     elevatorSynchronization.elevatorRequestsLock->Acquire();
//     int amountOfElevatorUsersOnCurrentFloor = requestPerFloor[currentFloor - 1];
//     elevatorSynchronization.elevatorRequestsLock->Release();

    elevatorSynchronization.currentFloorLock->Acquire();
    elevatorSynchronization.attemptToEnterElevator->Broadcast(elevatorSynchronization.currentFloorLock);
    elevatorSynchronization.currentFloorLock->Release();

//     elevatorSynchronization.closeDoorLock->Acquire(); // As soon as all expected elevator users enter elevator close
//                                                       // door.
//     while(amountOfElevatorUsersOnCurrentFloor != 0){
//         elevatorSynchronization.elevatorUsersEnteredElevator->Wait(elevatorSynchronization.closeDoorLock);
//         amountOfElevatorUsersOnCurrentFloor--;
//     }
// 
//     elevatorSynchronization.closeDoorLock->Release();
}

void
SimpleElevator::MoveFloors() {                       // Moves elevator a floor closer to the target floor.

    elevatorSynchronization.currentFloorLock->Acquire();

    for (int i = 0; i < 50000000; i++) {} //simulate the elevator moving
    if (elevatorDirection == UP) {
        currentFloor++;
    } else {
        currentFloor --;
    }

    // TODO: Might want to add an else if on line 73.
    if(elevatorDirection == UP && currentFloor == numberOfFloors){
        elevatorDirection = DOWN;
    }

    if(elevatorDirection == DOWN && currentFloor == 0){
         elevatorDirection = UP;
    }
    
    elevatorSynchronization.currentFloorLock->Release();
}

void 
SimpleElevator::DecrementElevatorCapacity() {        // Decrements the capacity of the elevator by 1.

    if (currentCapacity < 5) {
        currentCapacity++;
    }
}

void 
SimpleElevator::IncrementElevatorCapacity() {       // Increments the capacity of the elevator by 1.
    
    if (currentCapacity > 0) {
        currentCapacity--;
    }
}

int 
SimpleElevator::ElevatorDirection(){ // Gets the elevator direction, an elevator can be going up or down.

    return elevatorDirection;
}

void 
SimpleElevator::RequestElevator(int floor){ //Requests an elevator.

    numberOfElevatorRequest++;
    requestPerFloor[floor - 1]++;
}

void 
SimpleElevator::RemoveElevatorRequest(int floor) { // Removes an elevator request.

    numberOfElevatorRequest--;
    requestPerFloor[floor - 1]--;
}

ElevatorDirection 
SimpleElevator::GetElevatorDirection(){

    return elevatorDirection;
}

int 
SimpleElevator::GetNumberOfElevatorRequest() {

    return numberOfElevatorRequest;
}

int 
SimpleElevator::GetCurrentFloor() {

    return currentFloor;
}

bool 
SimpleElevator::ElevatorHasSpace() {

    return currentCapacity < 5;
}
