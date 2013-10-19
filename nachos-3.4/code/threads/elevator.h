#ifndef HW1_Elevator
#define HW1_Elevator
#include "synch.h"

enum ElevatorDirection {UP, DOWN};
// A struct to represent an elevator user. An elevator request consist of the current floor of the elevator user
// and the destination floor and an id.

struct ElevatorUser {
    int fromFloor;
    int toFloor;
    int id;
};


// A struct to maintina elevator synchronization mechanisms.
struct {
    Condition* arrivedToNewFloor;
    Condition* attemptToEnterElevator;
    Condition* elevatorUsersEnteredElevator;
    Condition* elevatorIsActive;
    Lock* elevatorRequestsLock;
    Lock* enterElevatorLock;
    Lock* closeDoorLock;
    Lock* currentCapacityLock;
    Lock* currentFloorLock;
 }  elevatorSynchronization =  { new Condition("Arrived to new Floor condition."),
                                 new Condition("Attempt to enter elevator condition."),
                                 new Condition("Elevator users entered elevator condition."),
                                 new Condition("Elevator is active condition."),
                                 new Lock("Lock for elevator request."), 
                                 new Lock("Lock for entering elevator."),
                                 new Lock("Lock for closing elevator door."),
                                 new Lock("Lock for current capacity."),
                                 new Lock("Lock for the current floor.")};


// A class to represent an elevator. An elevator satisfies request to take elevator users from one floor to another.
class SimpleElevator {
public:
    SimpleElevator(int numberOfFloors); // Creates an instance of an Elevator object with the specified number of floors.
    ~SimpleElevator(); // Releases resources of an Elevator object.
    void OpenAndCloseDoors(); // Opens the doors of the elevator to allow current passangers to get out and new one to get in.
    void MoveFloors(); // Moves elevator a floor closer to the target floor.
    int ElevatorDirection(); // Gets the elevator direction, an elevator can be going up or down.
    void RequestElevator(int floor); //Requests an elevator.
    void DecrementElevatorCapacity(); // Decrements the capacity of the elevator by 1.
    void IncrementElevatorCapacity(); // Increments the capacity of the elevator by 1.
    enum ElevatorDirection GetElevatorDirection(); // Gets the current elevator direction.
    void RemoveElevatorRequest(int floor); // Removes elevator request from list.
    int GetNumberOfElevatorRequest(); // Gets number of pending elevator request.
    int GetCurrentFloor(); // Gets current floor number.
    bool ElevatorHasSpace(); // True if the elevator has space(ie capacity < 5)

private:
    int currentFloor; // The current floor of the elevator.
    int numberOfFloors; // The number of floors the elevator can reach.
    int currentCapacity; // The current capacity of the elevator.
    int numberOfElevatorRequest; // All the request for the elevator.
    int * requestPerFloor; // Number of request per floor.
    enum ElevatorDirection elevatorDirection; // The direction the elevator is heading. Either up or down.
};

#endif //HW1_Elevator
