#include "defs.h"



//function prototypes
void test();
void PrintEvidence(HouseType * h);
void findGhost(int e1,int e2,int e3,int e4);



int main()
{
    //Initialize the random number generator
    srand(time(NULL));

   
    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
    initHouse(&house);

    //create temp hunter vars
    HunterType *h1,*h2,*h3,*h4;
    


   
    //Initialize hunters
    //current room to the head of the room list as its the van
    int takenEquipment[NUM_HUNTERS] = {0,0,0,0};
    initHunter(&h1, EMF,house.rooms.head->room,&house.evidenceList,takenEquipment);
    initHunter(&h2, TEMPERATURE,house.rooms.head->room,&house.evidenceList,takenEquipment);
    initHunter(&h3, FINGERPRINTS,house.rooms.head->room,&house.evidenceList,takenEquipment);
    initHunter(&h4, SOUND,house.rooms.head->room,&house.evidenceList,takenEquipment);

    // Assign the initialized hunters to the house's hunters array
    house.hunters[0] = h1;
    house.hunters[1] = h2;
    house.hunters[2] = h3;
    house.hunters[3] = h4;

   
   


    // Make the threads
    pthread_t t_h1, t_h2, t_h3, t_h4, t_g;



    printf("starting game \n-------------\n\n\n");

    //create each thread and start them
    if(pthread_create(&t_h1,NULL,&hunterThread,house.hunters[0]) != 0) return 0;
    if(pthread_create(&t_h2,NULL,&hunterThread,house.hunters[1]) != 0) return 0;
    if(pthread_create(&t_h3,NULL,&hunterThread,house.hunters[2]) != 0) return 0;
    if(pthread_create(&t_h4,NULL,&hunterThread,house.hunters[3]) != 0) return 0;
    if(pthread_create(&t_g,NULL,&ghostThread,house.ghost) != 0) return 0;


    //join the threads when they end
    if(pthread_join(t_h1,NULL) != 0) return 0;
    if(pthread_join(t_h2,NULL) != 0) return 0;
    if(pthread_join(t_h3,NULL) != 0) return 0;
    if(pthread_join(t_h4,NULL) != 0) return 0;
    if(pthread_join(t_g,NULL) != 0) return 0;


    printf("\n\nEND OF SIMULATION\n-----------------\n");

    // Initialize a counter for hunters who have exited
    int count = 0;

    // Print the names of hunters who have exited due to fear
    printf("Exited Due to FEAR:\n");
    for(int i = 0; i < NUM_HUNTERS; i++){
        if(house.hunters[i]->fear >= FEAR_MAX){
            printf("    %s\n",house.hunters[i]->name);
            count++;
        }
    }
    // Print the names of hunters who have exited due to boredom
    printf("\nExited Due to BOREDOM:\n");
    for(int i = 0; i < NUM_HUNTERS; i++){
        if(house.hunters[i]->boredom >= BOREDOM_MAX){
            printf("    %s\n",house.hunters[i]->name);
            count++;
        }
    }
    // If at least 4 hunters have exited, the ghost wins
    if(count >= 4){
        printf("\nGHOST has WON!!\n\n");
    }
    // Print the evidence found in the house
    PrintEvidence(&house);


    // Clean up the house
    cleanUpHouse(&house);


    return 0;
}


/*
 Function: test

 Function to test the initialization and connectivity of a house.

 Parameters: None.

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void test(){

    // Initialize a house
    HouseType house;
    initHouse(&house);
    
    
    // Populate rooms in the house
    populateRooms(&house);
    // Traverse through the rooms and print their names


    RoomNodeType *node = house.rooms.head;

    node = node->next;


    // 
    printf("Hunters are: %s\n",house.hunters[0]->name);
    printf("Hunters are: %s\n",house.hunters[1]->name);
    printf("Hunters are: %s\n",house.hunters[2]->name);
    printf("Hunters are: %s\n",house.hunters[3]->name);
    printf("Your team is in room: %s\n",house.hunters[0]->currentRoom->name);

    // Print the rooms connected to the current room
    printf("%s connects to: \n",node->room->name);
    node = node->room->connectedRooms.head;
    while(node->next != NULL){
        
        printf("    %s\n",node->room->name);
        node = node->next;
    }
   


    

}

/*
 Function: PrintEvidence

 Prints the evidence collected in the house and checks for unique evidence.

 Parameters:
   in h - Pointer to the HouseType containing the evidence.
   in/out uniqueEvidence - used to hold the number of unique evidence
   in/out e1,e2,e3,e4 - used to make sure only one evidince of the same type is counted
   in/out node - used to go through the evidince linked list

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

// Function to print the evidence collected in the house
void PrintEvidence(HouseType *h){

    // Initialize a counter for unique evidence and flags for each type of evidence
    int uniqueEvidence = 0;
    int e1 = 0,e2 = 0,e3 = 0,e4 = 0;
    printf("Evidence list:\n");
    // Initialize a node pointer to the head of the house's evidence list
    EvidenceNodeType * node = h->evidenceList.head;
    if(h->evidenceList.head== NULL)return;

    while(node->next != NULL){

        switch (node->evidence)
        {

        case EMF:
            if(e1 == 0){
                uniqueEvidence++;
                e1 = 1;
            }
            break;

        case TEMPERATURE:
            if(e2 == 0){
                uniqueEvidence++;
                e2 = 1;
            }
            break;

        case FINGERPRINTS:
            if(e3 == 0){
                uniqueEvidence++;
                e3 = 1;
            }
            break;

        case SOUND:
            if(e4 == 0){
                uniqueEvidence++;
                e4 = 1;
            }
            break;
        
        default:
            break;
        }

        // Print the number of unique pieces of evidence found
        printf("%d, ",node->evidence);
        node = node->next;
    }


    printf("\nUNIQUE evidence = %d\n",uniqueEvidence);
    if(uniqueEvidence == 3){

        findGhost(e1,e2,e3,e4);

    }
}

/*
 Function: findGhost

 Determines the type of ghost based on the evidence found.

 Parameters:
   in e1,e2,e3,e4 - used to store which type of evidence are in the list

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

// Function to find the ghost based on the evidence found
void findGhost(int e1,int e2,int e3,int e4) {


    //check for the three evidences to match to a ghost
    if(e4 == 1 && e2 == 1 && e3 == 1){
        printf("the ghost is PHANTOM\n");

    }
    else if(e1 == 1 && e2 == 1 && e4 == 1){
        printf("the ghost is BANSHEE\n");

    }
    else if(e1 == 1 && e2 == 1 && e3 == 1){
        printf("the ghost is POLTERGEIST\n");

    }
    else if(e1 == 1 && e3 == 1 && e4 == 1){
        printf("the ghost is BULLIES\n");

    }
    else{
        printf("the ghost is UNKNOWN\n");
    }
}