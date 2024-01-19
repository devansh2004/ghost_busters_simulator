#include "defs.h" 


//function prototypes
void collectEvidence(HunterType *h);
void moveHunter(HunterType *h);
void reviewEvidence(HunterType *h);
void pickEquipment(HunterType *h, int * taken);



/*
 Function: hunterThread

 Thread function for simulating the behavior of a hunter in a house.

 Parameters:
   in arg - Pointer to the HunterThreadArg structure containing the house and hunter.
   in/out h - used to store the hunter from arg

 Returns: 
   (void*) - Always returns NULL (as required by the pthread library).
*/


void *hunterThread(void *arg){
    //change the void arg to the struct type with the actual
    HunterType *h = (HunterType*)arg;

    // Infinite loop
    while(1){
        usleep(HUNTER_WAIT);
        printf("\n");
        // If there's a ghost in the current room
        if(h->currentRoom->ghost != NULL) {
            h->fear++;
            h->boredom = 0;
        
            
            
        }
        else{
            // If there's no ghost, increase the hunter's boredom level
            h->boredom++;
        }

        // If the hunter's fear or boredom level reaches the maximum
        if(h->fear >= FEAR_MAX || h->boredom >=BOREDOM_MAX){

            // The hunter leaves the current room
            h->currentRoom->huntersInRoom[h->equipment] = NULL;

            // Log the hunter's exit
            if(h->fear >= FEAR_MAX){
            l_hunterExit(h->name,LOG_FEAR);
            }
            else{
                l_hunterExit(h->name,LOG_BORED);
            }

            // Return 0 to show the hunter has left
            return 0;
        }

        //randInt(0,3)
        switch (randInt(0,3))
        {
        case 0:
            moveHunter(h);
            break;

        case 1:
            collectEvidence(h);
            break;

        case 2:
            reviewEvidence(h);
            break;
        
        default:
            break;
        }

    
             
    }
    
}


/*
 Function: initHunter

 Initializes a HunterType, allocating memory and setting initial values.

 Parameters:
   in/out h - Pointer to a pointer for the HunterType to be initialized.
   in evidenceType - The type of evidence the hunter is equipped with.
   in room - Pointer to the RoomType where the hunter is located.
   in evidenceList - Pointer to the EvidenceListType for the hunter's evidence.
   in taken - stores the value of taken equipment

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void initHunter(HunterType **h, EvidenceType evidenceType, RoomType *room, EvidenceListType * evidenceList, int *taken){

    //alocate memory for the hunter
    (*h) = (HunterType*) malloc(sizeof(HunterType));

    //make sure the memory is alocated and the hunter is not null
    if(h == NULL) return;

    //add the values for the hunter and get user to input a name for the hunter
    (*h)->boredom = 0;
    //(*h)->equipment = evidenceType;
    (*h)->fear = 0;
    
    char name[MAX_STR] = "";

    printf("what is the hunter's name?: ");
    scanf("%s",name);


    strcpy((*h)->name,name);
    

    pickEquipment((*h),taken);


    (*h)->evidenceList = evidenceList;

    (*h)->currentRoom = room;

    room->huntersInRoom[(*h)->equipment] = (*h);

    //tell the logger we initiated an hunter
    l_hunterInit(name,evidenceType);
    printf("\n");
}


/*
 Function: collectEvidence

 Collects evidence from the current room if it matches the hunter's equipment.

 Parameters:
   in/out h - Pointer to the HunterType who is collecting evidence.
   in/out node - node to go thorugh the evidince linked list

 Returns: 
   (void) - Nothing is returned; it's a void function.

*/




// Function for a hunter to collect evidence
void collectEvidence(HunterType *h){
    
    if(h == NULL)return;

    // Initialize a node pointer to the head of the evidence list in the current room
    EvidenceNodeType *node = h->currentRoom->evidenceList.head;

    
    if(node == NULL)return;

    // Traverse the list until a node with the same evidence as the hunter's equipment is found
    while(node->next != NULL){
        if(h->equipment == node->evidence){
            break;
        }

        node = node->next;
    }


    // If no node with the same evidence as the hunter's equipment is found, return immediately
    if(node->evidence != h->equipment){
        return;
    }

    // Wait for the mutex to be available and then lock it
    sem_wait(&h->evidenceList->mutex);

    // Add the evidence to the hunter's evidence list
    addEvidence(h->evidenceList,node->evidence);

    // Delete the evidence from the current room's evidence list
    deleteEvidence(&h->currentRoom->evidenceList,node->evidence);

    // Unlock the mutex
    sem_post(&h->evidenceList->mutex);

    // Log the hunter's collection of evidence
    l_hunterCollect(h->name,h->equipment,h->currentRoom->name);
}

/*
 Function: moveHunter

 Moves a hunter to a randomly selected connected room.

 Parameters:
   in/out h - Pointer to the HunterType to be moved.
   in/out r - random int to see which room the hunter moves too
   io/out node - used to get to the room the hunter will move into

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void moveHunter(HunterType *h){
    //get a random room
    int r = randInt(0,h->currentRoom->connectedRooms.size);

    RoomNodeType *node = h->currentRoom->connectedRooms.head;



    //get to random room in linked list
    for(int i = 0; i < r; i++){
        if(node->next != NULL){
            node = node->next;
        }
        else{
            break;
        }
        
    }

    // The hunter leaves the current room
    h->currentRoom->huntersInRoom[h->equipment] = NULL;

    h->currentRoom = node->room;

    node->room->huntersInRoom[h->equipment] = h;

    // Log the hunter's movement
    l_hunterMove(h->name,h->currentRoom->name);
}


/*
 Function: reviewEvidence

 Allows a hunter to review evidence and exit the room if three unique pieces are found.

 Parameters:
   in/out h - Pointer to the HunterType reviewing evidence.
   in/out uniqueEvidence - used to hold the number of unique evidence
   in/out e1,e2,e3,e4 - used to make sure only one evidince of the same type is counted
   in/out node - used to go through the evidince linked list

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

// Function for a hunter to review evidence
void reviewEvidence(HunterType *h){

    // Initialize a counter for unique evidence and flags for each type of evidence
    int uniqueEvidence = 0;
    int e1 = 0,e2 = 0,e3 = 0,e4 = 0;

    // Initialize a node pointer to the head of the hunter's evidence list
    EvidenceNodeType * node = h->evidenceList->head;
    if(h->evidenceList->head == NULL)return;

    // change the list until three unique pieces of evidence are found
    while(node->next != NULL && uniqueEvidence != 3){

        // Check the type of evidence in the current node
        switch (node->evidence)
        {
 
        // first EMF evidence found, increment the unique evidence counter
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
        node = node->next;
    }

    // If three unique pieces of evidence are found
    if(uniqueEvidence == 3){
        h->currentRoom->huntersInRoom[h->equipment] = NULL;
        l_hunterReview(h->name,LOG_SUFFICIENT);
        // Log the hunter's exit due to finding enough evidence
        l_hunterExit(h->name,LOG_EVIDENCE);
        pthread_exit(NULL);
    }

    l_hunterReview(h->name,LOG_INSUFFICIENT);

}


/*
 Function: pickEquipment

 Allows a hunter to pick a piece of equipment if it's not already taken.

 Parameters:
   in/out h - Pointer to the HunterType picking equipment.
   in/out taken - Pointer to an array tracking which pieces of equipment have been taken.
   in/out i - used to make the while loop work
   in/out choice - used to get the user input on what equipment they want

 Returns: 
   (void) - Nothing is returned; it's a void function.

*/

// Function to allow a hunter to pick equipment
void pickEquipment(HunterType *h, int * taken){
    // Initialize loop control variable
    int i = C_TRUE;
    // Variable to store the user's choice
    int choice = 0;
    // Loop until a valid choice is made
    while(i != C_FALSE){
        // Prompt the user to pick equipment
        printf("pick equipment ((1)EMF, (2)Temperature, (3)Finger prints, (4) Sound): ");
        // Read the user's choice
        scanf("%d", &choice);
        // Process the user's choice
        switch (choice)
        {
        case 1:
            // If the EMF equipment is not already taken
            if(taken[0] != 1){
                // Assign the EMF equipment to the hunter
                h->equipment = EMF;
                // Mark the EMF equipment as taken
                taken[0] = 1;
                // Exit the loop
                i = C_FALSE;
            }
            else{
                printf("invalid input(equipment is taken)\n");
            }
            break;
        
        case 2:
            if(taken[1] != 1){
                h->equipment = TEMPERATURE;
                taken[1] = 1;
                i = C_FALSE;
            }
            else{
                printf("invalid input(equipment is taken)\n");
            }
            break;
        
        case 3:
            if(taken[2] != 1){
                h->equipment = FINGERPRINTS;
                taken[2] = 1;
                i = C_FALSE;
            }
            else{
                printf("invalid input(equipment is taken)\n");
            }
            break;
        
        case 4:
            if(taken[3] != 1){
                h->equipment = SOUND;
                taken[3] = 1;
                i = C_FALSE;
            }
            else{
                printf("invalid input(equipment is taken)\n");
            }
            break;
        default:
            printf("invalid input\n");
            break;
        }
    }
}