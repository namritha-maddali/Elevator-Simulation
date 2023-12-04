/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

/* Function Declarations */

// function to handle the external requests
void handleExternalRequests(QUEUE_UP *goingUpQueue, QUEUE_DOWN *goingDownQueue, int numberOfFloors, int choice);

// funtion to handle the internal requests
void handleInternalRequests(QUEUE_UP *goingUpQueue, QUEUE_DOWN *goingDownQueue, int *floorReached, int numberOfFloors);

// function to carry out entire simulation of the elevator
void elevatorSimulation(QUEUE_UP *goingUpQueue, QUEUE_DOWN *goingDownQueue, int numberOfFloors);


int main() {

    // Initializing the priority queues
    QUEUE_UP goingUpQueue;
    QUEUE_DOWN goingDownQueue;
    initializeUp(&goingUpQueue);
    initializeDown(&goingDownQueue);

    int choice = 0;  // to ask if the lift is being called again after one entire cycle of going up and down
    int numberOfFloors = 0;

    printf("Enter the number of floors: ");
    scanf("%d", &numberOfFloors);

    /* printf("number of destinations = %d\n",(numberOfFloors)*2); */

    srand(time(NULL));  // seed for random number generation

    do {
        /* printf("in the while loop\n"); */

        // the external requests are generated first and then the elevator starts moving 
        handleExternalRequests(&goingUpQueue, &goingDownQueue, numberOfFloors, choice);
        elevatorSimulation(&goingUpQueue, &goingDownQueue, numberOfFloors);

        printf("\nDo you want to generate another list of external requests for the elevator?\nIf yes, type '1' else press any other number\n");
        scanf("%d", &choice);;

    } while(choice == 1);

    printf("Thank you for using our elevator! Have a nice day :)\n");
    return 0;
}

/* Function Definitions */

void handleExternalRequests(QUEUE_UP *goingUpQueue, QUEUE_DOWN *goingDownQueue, int numberOfFloors, int choice) {
    /* printf("now handling external requests\n"); */
    int sizeOfExtReqList,extFloor,direction;

    if(choice){
        // randomly generating external requests

        sizeOfExtReqList = rand() % (numberOfFloors*2);  // Assuming a set value for the number of times the elevator is being called 
        /* printf("\nnumber of external requests to the elevator:: %d\n", sizeOfExtReqList); */

        // to randomly generate the floor in which the elevator is being called and which direction they want to go to
        for (int i = 0; i < sizeOfExtReqList; i++) {    
            extFloor = rand() % (numberOfFloors+1);  // randomly choosing a floor from 0 to last floor
            if(extFloor == 0)
                direction = 1;      // the elevator cannot go down from ground floor
            else if(extFloor == numberOfFloors)
                direction = -1;     // the elevator cannot go up beyond the last floor
            else
                direction = rand() % 2 == 0 ? 1 : -1;  // randomly choosing direction (up or down)
            
            /* printf("%d %d\n",extFloor,direction); */

            // enqueuing the external requests wherever the passengers want to go up
            if (direction == 1) {
                /* printf("duplicates:: %d\n",checkDuplicatesUp(goingUpQueue,extFloor)); */
                if (!checkDuplicatesUp(goingUpQueue,extFloor)){ //if there are no duplicates then enqueue
                    enqueueUp(goingUpQueue, extFloor);
                    /* printf("enqueued external req in up\n"); */
                }
                else continue; // the loop should continue the next request if there are any duplicates found
            } 
            // enqueuing the external requests wherever the passengers want to go down
            else {     // direction == -1
                /* printf("duplicates:: %d\n",checkDuplicatesDown(goingDownQueue,extFloor)); */
                if (!checkDuplicatesDown(goingDownQueue,extFloor)){ //if there are no duplicates then enqueue
                    enqueueDown(goingDownQueue, extFloor);
                    /* printf("enqueued external req in down\n"); */
                }
                else continue;
            }
        }
    }
}

void handleInternalRequests(QUEUE_UP *goingUpQueue, QUEUE_DOWN *goingDownQueue, int *floorReached, int numberOfFloors) {
    int internalReq;
    int numberOfPeoplePerFloor; // number of people getting on on each floor
    /* printf("now handling internal requests\n"); */

    while (goingUpQueue->head != NULL || goingDownQueue->head != NULL) {    // while the requests are enqueued
        int state = (*floorReached == 0) ? 1 : ((*floorReached == numberOfFloors) ? -1 : 1);    // state of the elevator is 1 by default

        if (state == 1) {   // the elevator is moving up
            printf("\n\nNow going up\n");
            *floorReached = dequeueUp(goingUpQueue);
            while (goingUpQueue->head != NULL && *floorReached!=numberOfFloors) {
                printf("How many people are entering on floor %d? :: ", *floorReached);
                scanf("%d", &numberOfPeoplePerFloor);
                
                // adding destinations depending on the number of people entering
                for (int i = 0; i < numberOfPeoplePerFloor; i++) {
                    printf("Which floor would you like to go to? :: ");
                    scanf("%d", &internalReq);
                    if(internalReq > *floorReached){    // to not go to the floors below the current floor
                        enqueueUp(goingUpQueue, internalReq);
                    }
                }

                // dequeuing the floor when reached
                *floorReached = dequeueUp(goingUpQueue);
                printf("\nArrived at %d floor\n\n", *floorReached);
            }
            // to enable the state change of the elevator when it reaches a floor that isnt the last floor
            if (*floorReached!=numberOfFloors) { 
                printf("\n\nNow going down\n");
                enqueueUp(goingUpQueue, numberOfFloors);
                break;
            }

        }

        else if (state == -1) {
            // *floorReached = dequeueUp(goingUpQueue);
            printf("\n\nNow going down\n");
            while (goingDownQueue->head != NULL) {
                printf("How many people are entering on floor %d? :: ", *floorReached);
                scanf("%d", &numberOfPeoplePerFloor);
                
                // adding destinations depending on the number of people entering
                for (int i = 0; i < numberOfPeoplePerFloor; i++) {
                    printf("Which floor would you like to go to? :: ");
                    scanf("%d", &internalReq);
                    if(internalReq < *floorReached){    // to not go to the floors above the current floor
                        enqueueDown(goingDownQueue, internalReq);
                    }
                }

                // dequeuing the floor when reached
                *floorReached = dequeueDown(goingDownQueue);
                printf("\narrived at %d floor\n", *floorReached);
            }
            if(*floorReached!=numberOfFloors)
                state = 1;
        }
    }
}

void elevatorSimulation(QUEUE_UP *goingUpQueue, QUEUE_DOWN *goingDownQueue, int numberOfFloors) {
    int floorReached = 0;   // the ground floor is assumed to be the start 
    /* int totalNumberOfPeople = 0; */
    handleInternalRequests(goingUpQueue, goingDownQueue, &floorReached, numberOfFloors);
}
