
/*
    assumptions 
    1. the program is the lift and you are the watchman
    2. the lift by default starts from ground floor and is set to move in the up direction
    3. the lift resets the state when it finishes going up all together and is set to move till the last floor before starting its journey down and it's state to -1 after reaching the last floor
    4. the external requests are set randomly at the same time fir the elevator in the beginning or when the queue is cleared
    5. the limit to the number of external requests at a time is 2*(number of floors)
    
*/


#include <stdio.h>
#include <stdlib.h>
#include "header.h"

/* FUNCTION DEFINITIONS */

// the elevator is stationery at Ground floor but if it is not called by someone there, 
// it proceeds to the next floor automatically, hence first node is initialized to 0
void initializeUp(QUEUE_UP *elevatorGoingUp){
    NODE *newNode = malloc(sizeof(NODE));
    elevatorGoingUp->head = newNode;
    newNode->floor=0;
    newNode->next=NULL;
    // elevatorGoingUp->head = NULL;
}

void initializeDown(QUEUE_DOWN *elevatorGoingDown){
    elevatorGoingDown->head = NULL;
}


// to check if the internally or externally requested floor already exists in the queues 
int checkDuplicatesUp(QUEUE_UP *elevatorGoingUp, int floorToBeChecked) {
    NODE *traverse = elevatorGoingUp->head; // temporary instance of the existing queue for traversal

    while (traverse != NULL) {       // till the last element of the queue
        if (floorToBeChecked == traverse->floor) {
            return 1; // duplicate found
        }
        traverse = traverse->next;
    }
    return 0; // no duplicate found
}


int checkDuplicatesDown(QUEUE_DOWN *elevatorGoingDown, int floorToBeChecked){
    NODE *traverse = elevatorGoingDown->head; // temporary instance of the existing queue for traversal

    while (traverse != NULL) {      // till the last element of the queue
        if (floorToBeChecked == traverse->floor) {
            return 1; // Duplicate found
        }
        traverse = traverse->next;
    }
    return 0; // No duplicate found
}


// to add the floors to the queue in ascending order
void enqueueUp(QUEUE_UP *elevatorGoingUp, int request){
    // creating a new node
    NODE *newNode = malloc(sizeof(NODE));
    newNode->floor = request;
    newNode->next = NULL;

    // no values are added to the queue if duplicates are found
    if(checkDuplicatesUp(elevatorGoingUp,request)){    
        free(newNode);  // free the node to save space 
        return;
    }

    else{
        // creating temporary pointers to not change the original UP pointer argument
        NODE *temp1 = elevatorGoingUp->head;
        NODE *temp2 = NULL;
    
        // to traverse through the existing queue till the newNode fits in the order (ascending)
        while(temp1!=NULL && (temp1->floor < newNode->floor)){
            temp2 = temp1;
            temp1 = temp1->next;
        }

        /*
            head pointer = temp1 
            consider it 2
            if newnode is 1
            then the temp2 value remains null

            temp1 = [ 1 3 5 7 9 ]
            add 4 = newNode->floor

            1 < 4 : true 
            temp2 = [ 1 temp1 ]
            temp1 = [3 5 7 9]
            3 < 4 : true 
            temp2 = [ 3 temp1 ]
            temp1 = [ 5 7 9 ]
            5 < 4 : false

            temp2->next = newNode;
            temp2 = [ 3 newNode ]
            newNode->next = temp1;

            finally the queue becomes :: 
            [3 newNode temp1 ]
        */

        // if(temp1->floor > newNode->floor){;}

        // if the queue is empty or the floor entered is less than all other values of the queue
        if(temp2 == NULL){
            newNode->next = elevatorGoingUp->head;
            elevatorGoingUp->head = newNode;
        }
        // to add the floor value in the middle or the end of the queue
        else{
            newNode->next = temp1;
            temp2->next = newNode;
        }
    }
}

// to add the floors to the queue in descending order
void enqueueDown(QUEUE_DOWN *elevatorGoingDown, int request){
    // creating a new node
    NODE *newNode = malloc(sizeof(NODE));
    newNode->floor = request;
    newNode->next = NULL;

    // no values are added to the queue if duplicates are found
    if(checkDuplicatesDown(elevatorGoingDown,request)){   
        free(newNode);
        return;
    }

    else{
        // creating temporary pointers to not change the original UP pointer argument
        NODE *temp1 = elevatorGoingDown->head;
        NODE *temp2 = NULL;

        // to traverse through the existing queue till the newNode fits in the order (descending)
        while(temp1!=NULL && (temp1->floor > newNode->floor)){
            temp2 = temp1;
            temp1 = temp1->next;
        }

        // if(temp1->floor < newNode->floor){;}

        // if the queue is empty or the floor entered is greater than all other values in the queue
        if(temp2 == NULL){
            newNode->next = elevatorGoingDown->head;
            elevatorGoingDown->head = newNode;
        }

        // to add the floor value in the middle or the end of the queue
        else{
            newNode->next = temp1;
            temp2->next = newNode;
        }
    }
}


// to dequeue the floors from the queues everytime the floor is reached
int dequeueUp(QUEUE_UP *elevatorGoingUp){
    NODE *reachedFloor = elevatorGoingUp->head;     // a temporary pointer to say which floor has been reached
    
    int floorVal = reachedFloor->floor;    // this is to return the floor which was reached 
    elevatorGoingUp->head = elevatorGoingUp->head->next;
    free(reachedFloor);     // freeing the memory location where the reached floor is at

    return floorVal;
}

int dequeueDown(QUEUE_DOWN *elevatorGoingDown){
    NODE *reachedFloor = elevatorGoingDown->head;

    int floorVal = reachedFloor->floor;
    elevatorGoingDown->head = elevatorGoingDown->head->next;
    free(reachedFloor); 

    return floorVal;
}

/*
void destroyUpQueue(QUEUE_UP *elevatorGoingUp){
    QUEUE_UP *traverse;
    while(elevatorGoingUp!=NULL){
        traverse = elevatorGoingUp->head->next;
        free(elevatorGoingUp->head);
        elevatorGoingUp->head = traverse;
    }
    printf("last floor reached, please enter the destinations again\n");
}

void destroyDownQueue(QUEUE_DOWN *elevatorGoingDown){
    QUEUE_DOWN *traverse;
    while(elevatorGoingDown!=NULL){
        traverse = elevatorGoingDown->head->next;
        free(elevatorGoingDown->head);
        elevatorGoingDown->head = traverse;
    }
    printf("ground floor reached, please enter the destinations again\n");
}
*/