#include "elevator.h"
#include "system.h"
#include <math>

// Creates an instance of an Elevator object with the specified number of floors.
Elevator::Elevator(int numberOfFloors){

    Elevator::numberOfFloors = numberOfFloors;
    currentCapacity = 5;
    currentFloor = 1;
    elevatorDirection = UP;
}

// Releases resources of an Elevator object.
Elevator::~Elevator(){
    delete elevatorRequests;
}


// Opens the doors of the elevator to allow current passangers to get out and new one to get in.
void Elevator::OpenDoors(){
    elevatorSynchronization.arrivedToNewFloor->Broadcast(); // Wakes up elevator users in the elevator so they can leave if 
                                                            // elevator arrived to their requested floor.

    int amountOfElevatorUsersOnCurrentFloor = 0;
    elevatorSynchronization.elevatorRequestsLock->Acquire();

    for(int i = 0; i < elevatorRequests.size();i++){

        ElevatorRequest elevatorRequest = elevatorRequests[i];

        //TODO: Should be able to refactor into less statements by adding if condition as loop variable.
        if(elevatorRequest.currentFloor == currentFloor &&  
           elevatorDirection == DetermineDirection(elevatorRequest.currentFloor, elevatorRequest.destinationFloor)){
                                                    // Counts the amount of people that are in current floor and are
            amountOfElevatorUsersOnCurrentFloor++;  // heading in same direction as elevator.
        }

        elevatorSynchronization.
        while(amountOfElevatorUsersOnCurrentFloor != 0)
    }
    
    waitingElevatorUsers[currentFloor - 1]->Broadcast(); // Wakes up elevator users waiting for elevator on current
}

ElevatorDirection DetermineDirection(int currentFloor, int destinationFloor){
    return currentFloor < destinationFloor ? UP : DOWN;
}
void MoveFloors(); // Moves elevator a floor closer to the target floor.
int ElevatorDirection(); // Gets the elevator direction, an elevator can be going up or down.
void RequestElevator(ElevatorRequest elevatorRequest); //Requests an elevator.
void DecrementElevatorCapacity(); // Decrements the capacity of the elevator by 1.
void IncrementElevatorCapacity(); // Increments the capacity of the elevator by 1.
