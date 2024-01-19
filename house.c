#include "defs.h"

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(HouseType* house) {
    // First, create each room
    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");
   
    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}


/*
 Function: initHouse

 Initializes a HouseType, including the list of rooms and a ghost.

 Parameters:
   in/out h - Pointer to the HouseType to be initialized.
   in/out node - used to loop through the list of rooms

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void initHouse(HouseType *h){

    // Initialize the list of rooms in the house
    initRoomList(&h->rooms);

    populateRooms(h);

     //add ghost to random room;

        //get a random room
        int r = randInt(1,h->rooms.size+1);
    
        RoomNodeType *node = h->rooms.head;

        //skip first room as its the van
        node = node->next;


        //get to random room in linked list
        for(int i = 0; i < r; i++){
            if(node->next != NULL){
                node = node->next;
            }
            else{
                break;
            }
            
        }

    // Initialize ghosts with unknown type and no initial room
    initGhost(&h->ghost, randomGhost(), node->room);

    initEvidenceList(&h->evidenceList);

/*
    for(int i = 0; i < MAX_HUNTERS; i++){
        initHunter(&h->hunters[i], "", i, NULL);
    }
*/
}


/*
 Function: cleanUpHouse

 Cleans up memory in a HouseType, including evidence, room data, room, and hunters.

 Parameters:
   in/out h - Pointer to the HouseType to be cleaned up.

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

// Function to clean up all data in the house
void cleanUpHouse(HouseType *h){

    // Clean up the evidence list in the house, data, room
    cleanUpEvidence(&h->evidenceList);
    cleanUpRoomData(&h->rooms);
    cleanUpRoom(&h->rooms);
    
    // Loop through all hunters in the house
    for(int i =0; i < NUM_HUNTERS; i++){
        free(h->hunters[i]);
    }

    // Free the ghost in the house
    free(h->ghost);


}
    
    




