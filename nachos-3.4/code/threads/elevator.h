#include <vector>
// A struct to represent an elevator request. An elevator request consist of the current floor of the elevator user
// and the destination floor.
enum ElevatorDirection {UP, DOWN};

struct ElevatorRequest{
    int currentFloor,
    int destinationFloor
};

// A struct to maintina elevator synchronization mechanisms.
struct ElevatorSynchronization{
    Lock* elevatorRequestsLock,
    Condition* arrivedToNewFloor,
    Condition* elevatorUsersEnteringElevator,
} elevatorSynchronization = { new Lock("Arrived to new floor."), 
                              new Condition("Elevator User inside elevator condition."),
                              new Condition("Elevator user entering elevator.")}


// A class to represent an elevator. An elevator satisfies request to take elevator users from one floor to another.
class Elevator {
public:
    Elevator(int numberOfFloors, ElevatorSynchronization* elevatorSynchronization); // Creates an instance of an Elevator object with the specified number of floors.
    ~Elevator(); // Releases resources of an Elevator object.
    void OpenDoors(); // Opens the doors of the elevator to allow current passangers to get out and new one to get in.
    void MoveFloors(); // Moves elevator a floor closer to the target floor.
    int ElevatorDirection(); // Gets the elevator direction, an elevator can be going up or down.
    void RequestElevator(ElevatorRequest elevatorRequest); //Requests an elevator.
    void DecrementElevatorCapacity(); // Decrements the capacity of the elevator by 1.
    void IncrementElevatorCapacity(); // Increments the capacity of the elevator by 1.
    void SetElevatorDirection(ElevatorDirection elevatorDirection);
    void GetElevatorDirection { return elevatorDirection;};

private:
    int currentFloor; // The current floor of the elevator.
    int numberOfFloors; // The number of floors the elevator can reach.
    int currentCapacity; // The current capacity of the elevator.
    std::vector<ElevatorUser*> elevatorRequests; // All the request for the elevator.
    ElevatorDirection elevatorDirection; // The direction the elevator is heading. Either up or down.
};
