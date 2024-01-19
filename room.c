#include "defs.h"

/*
 Function: initRoomList

 Initializes a RoomListType, setting the head to NULL and size to 0.

 Parameters:
   in/out list - Pointer to the RoomListType to be initialized.

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/


void initRoomList(RoomListType *list){
    // If the list is NULL, exit the function
    if(list == NULL) return;

    // Set the head of the list to NULL
    list->head = NULL;

    // Set the size of the list to 0
    list->size = 0;

    //initilize the semphore
    sem_init(&list->mutex,0,1);
}


/*
 Function: createRoom

 Creates and initializes a RoomType with a given name.

 Parameters:
   in name - the name of the room.
   in/out r - the var for the room

 Returns: 
   out r - Pointer to the created RoomType. Returns NULL if memory allocation fails.
*/

RoomType* createRoom(char* name){

    //alocate memory for the room
    RoomType *r = (RoomType*) malloc(sizeof(RoomType));

    //make sure the room has been alocated memory and isnt null
    if(r == NULL) return NULL;

    r->ghost = NULL;

    // initializes a room with a given name and empty lists of connected rooms and evidence. 
    //It also defines a function to connect two rooms by adding each room to the other's list of connected rooms.
    strcpy(r->name,name);
    for(int i = 0; i < NUM_HUNTERS; i++){
        r->huntersInRoom[i] = NULL;
    }
    initRoomList(&r->connectedRooms);
    initEvidenceList(&r->evidenceList);

    return r;

}

/*
 Function: connectRooms

 Connects two rooms by adding each room to the other's list of connected rooms.

 Parameters:
   in r1 - Pointer to the first RoomType to be connected.
   in r2 - Pointer to the second RoomType to be connected.

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/


void connectRooms(RoomType *r1, RoomType  *r2){
    addRoom(&r1->connectedRooms, r2);
    addRoom(&r2->connectedRooms, r1);
}


/*
 Function: addRoom

 Adds a RoomType to a RoomListType.

 Parameters:
   in/out list - Pointer to the RoomListType where the room will be added.
   in room - Pointer to the RoomType to be added.
   in/out newNode - node that holds the new room
   in/out c - temp node used to loop through linked list

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void addRoom(RoomListType *list , RoomType *room){
    //make sure both arguments arent null
    if(list == NULL || room == NULL) return;
    
    //allocate memory for the new room
    RoomNodeType *newNode = (RoomNodeType*)malloc(sizeof(RoomNodeType));
    
    //make sure memory is alocated
    if(newNode == NULL) return; 
    

    //add the room data into the node and make sure it points to null
    newNode->room = room;
    newNode->next = NULL;

    //if the head of the list is null make the new node the head
    if(list->head == NULL){
        
        // If the list is empty, set the new node as the head and update size
        list->head = newNode;
        list->size++;
        return;
    }

    
    // Otherwise, insert the new node at the beginning of the list
    RoomNodeType *c = list->head;

    while(c->next != NULL){
        c = c->next;
    }
    c->next = newNode;
    list->size ++;
    
    
}


/*
 Function: cleanUpRoomData
 Cleans up room data in a RoomListType, freeing allocated memory.

 Parameters:
  in/out r - Pointer to the RoomListType to be cleaned up.
  in/out node - used to go through the linked list of rooms

 Returns: nothing is a void funcion
 */

// Function to clean up room data, freeing allocated memory
void cleanUpRoomData(RoomListType *r){
    // Check if the room list is empty
    if(r == NULL) return;

    // Initialize a pointer to the head of the room list
    RoomNodeType * node = r->head;

    // Loop through the rooms in the list
    while (node->next != NULL)
    {
        // Clean up connected rooms and evidence for the current room
        cleanUpRoom(&node->room->connectedRooms);
        cleanUpEvidence(&node->room->evidenceList);
        
        // Move to the next room in the list
        node = node->next;
    }

    // Clean up connected rooms and evidence for the last room in the list
    cleanUpRoom(&node->room->connectedRooms);
    cleanUpEvidence(&node->room->evidenceList);

    // Reset the pointer to the head of the room list
    node = r->head;

    // Loop through the rooms in the list to free memory
    while (node->next != NULL)
    {
        // Free the memory allocated for the current room
        free(node->room);

        // Move to the next room in the list
        node = node->next;
    }

    // Free the memory allocated for the last room in the list
    free(node->room);    
}


/*
 Function: cleanUpRoom

 Cleans up room data in a RoomListType, freeing allocated memory.


 Parameters:
   in/out r - Pointer to the RoomListType to be cleaned up.
   in/out node - used to loop through the linked list
   in/out temp - used to hold the previous contents of node

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

// Function to clean up room data
void cleanUpRoom(RoomListType *r){
    if(r==NULL)return;
    RoomNodeType * node = r->head;

    RoomNodeType * temp = NULL;

    while (node->next != NULL)
    {
        temp = node->next;
        free(node);
        node = temp;
    }
    free(node);

    r->size = 0;
}