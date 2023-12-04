/*
    // creating a linked list for the external requests which are generated in random along with direction
    // button outside the lift at every floor
    typedef struct externalReq
    {
        int extFloor;
        int direction;
        struct externalReq *next;
    }NODE_EXTREQ;

    typedef struct hextreq
    {
        NODE_EXTREQ *head;
    }EXTREQ;

*/

// node for the destination the elevator must reach
typedef struct moving
{
    int floor;
    struct moving *next;
}NODE;

// ascending priority queue ~ for the ascent of the elevator (moving up)
typedef struct queueUp
{
    NODE *head;
}QUEUE_UP;

// descending priority queue ~ for the descent of the elevator (moving down)
typedef struct queueDown
{
    NODE *head;
}QUEUE_DOWN;


/* funcitions */

// to initialize the queues
void initializeUp(QUEUE_UP *elevatorGoingUp);
void initializeDown(QUEUE_DOWN *elevatorGoingDown);

// to check for duplicates in the requests being generated
int checkDuplicatesUp(QUEUE_UP *elevatorGoingUp, int floorToBeChecked);
int checkDuplicatesDown(QUEUE_DOWN *elevatorGoingDown, int floorToBeChecked);

// to enqueue the floor requests from both outside and inside the elevator
void enqueueUp(QUEUE_UP *elevatorGoingUp, int requests);
void enqueueDown(QUEUE_DOWN *elevatorGoingDown, int requests);

// to dequeue the floor values when reached by the elevator
int dequeueUp(QUEUE_UP *elevatorGoingUp);
int dequeueDown(QUEUE_DOWN *elevatorGoingDown);