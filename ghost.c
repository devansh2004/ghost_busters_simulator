#include "defs.h"


//function prototypes
void moveGhost(GhostType *g);

void leaveEvidence(GhostType* ghost);



/*
 Function: ghostThread

 Thread function for simulating the behavior of a ghost in a house.

 Parameters:
   in arg - Pointer to the GhostType ghost.
   in/out ghost - stores the ghost struct from arg
   in/out flag - used to run code if there is no hunter in the room

 Returns: 
   (void*) - Always returns NULL (as required by the pthread library).
*/

void *ghostThread(void * arg){

    //change the arg type to ghost and store it in a new var

    GhostType *ghost = (GhostType*)arg;


    while(1){
       usleep(GHOST_WAIT);
        int flag = 0;
        //check if ghost is in same room
        for(int i = 0; i < NUM_HUNTERS; i++){
            
            if(ghost->currentRoom->huntersInRoom[i] != NULL) {
                
                flag = 1;
                ghost->boredom = 0;


                //either add evidence or do nothing
                if(randInt(0,2) == 0){
                    break;
                }


                //add random peice of evidince
                sem_wait(&ghost->currentRoom->evidenceList.mutex);
                leaveEvidence(ghost);
                sem_post(&ghost->currentRoom->evidenceList.mutex);
                break;
                
            }
           
        }



        if(flag == 0){

            //increase the boredom
            ghost->boredom++;

            // Randomly decide what the ghost will do
            switch (randInt(0,3))
            {
            case 0:
                // The ghost leaves evidence
                sem_wait(&ghost->currentRoom->evidenceList.mutex);
                leaveEvidence(ghost);
                sem_post(&ghost->currentRoom->evidenceList.mutex);
                break;
            case 1:        
                // The ghost moves    
                moveGhost(ghost);
                break;     
            default:
                break;
            }

            flag = 0;
        }
        
        if(ghost->boredom >= BOREDOM_MAX){
            ghost->currentRoom->ghost = NULL;
            
            // Log the ghost's exit due to boredom
            l_ghostExit(LOG_BORED);
            return 0;
        }
      





        
    }


    
}


/*
 Function: initGhost

 Initializes a GhostType, allocating memory and setting initial values.

 Parameters:
   in/out g - Pointer to a pointer for the GhostType to be initialized.
   in ghostClass - The class of the ghost.
   in room - Pointer to the RoomType where the ghost is located.

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void initGhost(GhostType **g, enum GhostClass ghostClass, RoomType *room){
    
    //alocate memory for the Ghost
    (*g) = (GhostType*) malloc(sizeof(GhostType));
    
    //make sure the memory is alocated and the Ghost is not null
    if(g == NULL) return;
    
    //add the values for the Ghost
    (*g)->boredom = 0;
    (*g)->class = ghostClass;
    (*g)->currentRoom = room;

    room->ghost = (*g);
    
   

    //tell the logger we initiated an Ghost
    l_ghostInit(ghostClass, room->name);
}


/*
 Function: moveGhost

 Moves a ghost to a randomly selected connected room.

 Parameters:
   in/out g - Pointer to the GhostType to be moved.
   in/out node - used to go thorugh the linked list

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void moveGhost(GhostType *g){
    //get a random room
    int r = randInt(0,g->currentRoom->connectedRooms.size);

    RoomNodeType *node = g->currentRoom->connectedRooms.head;



    //get to random room in linked list
    for(int i = 0; i < r; i++){
        if(node->next != NULL){
            node = node->next;
        }
        else{
            break;
        }
        
    }

    g->currentRoom->ghost = NULL;
    g->currentRoom = node->room;
    g->currentRoom->ghost  = g;


    l_ghostMove(g->currentRoom->name);
}


/*
 Function: leaveEvidence

 Leaves evidence in the current room based on the ghost's type.

 Parameters:
   in ghost - Pointer to the GhostType that leaves evidence.
   in pol,ban,bul,pha - arrays used to store the types of evidince each ghost has
   in/out evidince = used the store the type of evidence the ghost will drop

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

// function for a ghost to leave evidence based on its type
void leaveEvidence(GhostType* ghost){

    // Define the possible evidence types for each ghost class
    EvidenceType pol[4] = {EMF,TEMPERATURE,FINGERPRINTS,EV_UNKNOWN};
    EvidenceType ban[4] = {EMF,TEMPERATURE,SOUND,EV_UNKNOWN};
    EvidenceType bul[4] = {EMF,FINGERPRINTS,SOUND,EV_UNKNOWN};
    EvidenceType pha[4] = {TEMPERATURE,FINGERPRINTS,SOUND,EV_UNKNOWN};
    EvidenceType evidence;

    // Switch statement based on the ghost's class
    switch (ghost->class)
        {
        case POLTERGEIST:
        // Select a random evidence type for a Poltergeist
        evidence = pol[randInt(0,4)];
            
            break;

        case BANSHEE:
            evidence = ban[randInt(0,4)];
            break;

        case BULLIES:
            evidence = bul[randInt(0,4)];
            break;

        case PHANTOM:
            evidence = pha[randInt(0,4)];
            break;

        default:
            break;
        }
        // Add the selected evidence to the ghost's current room's evidence list
        addEvidence(&ghost->currentRoom->evidenceList,evidence);

        // Log the evidence left by the ghost
        l_ghostEvidence(evidence,ghost->currentRoom->name);
}


