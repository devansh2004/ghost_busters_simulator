#include "defs.h"


/*
 Function: initEvidenceList

 Initializes an EvidenceListType, setting up the initial values for the list.

 Parameters:
   in/out list - Pointer to the EvidenceListType to be initialized.

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/


// Initializes an empty EvidenceListType
void initEvidenceList(EvidenceListType *list){
    if(list == NULL) return;

    list->head = NULL;
    list->size = 0;
    sem_init(&list->mutex,0,1);
}


/*
 Function: addEvidence

 Adds evidence to an EvidenceListType, creating a new node and updating the list.

 Parameters:
   in/out list - Pointer to the EvidenceListType to which evidence is added.
   in evidence - The evidence data to be added.
   in/out newNode - the node that holds the data for the new evidince
   in/out c - temp node used to loop through linked list

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

void addEvidence(EvidenceListType *list , EvidenceType evidence){
    //make sure both arguments arent null
    if(list == NULL) return;
    
    //allocate memory for the new room
    EvidenceNodeType *newNode = (EvidenceNodeType*)malloc(sizeof(EvidenceNodeType));
    
    //make sure memory is alocated
    if(newNode == NULL) return; 
    

    //add the room data into the node and make sure it points to null
    newNode->evidence = evidence;
    newNode->next = NULL;

    //if the head of the list is null make the new node the head
    if(list->head == NULL){
        
        // If the list is empty, set the new node as the head and update size
        list->head = newNode;
        list->size++;
        return;
    }

    
    // Otherwise, insert the new node at the beginning of the list
    EvidenceNodeType *c = list->head;

    while(c->next != NULL){
        c = c->next;
    }
    c->next = newNode;
    list->size ++;
    
    
}


/*
 Function: cleanUpEvidence

 Cleans up evidence data in an EvidenceListType, freeing allocated memory.

 Parameters:
   in/out e - Pointer to the EvidenceListType to be cleaned up.
   in/out node - used to loop through the linked list
   out temp - used to store the previous node in the linked list from node

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/

// Function to clean up all evidence nodes in the list
void cleanUpEvidence(EvidenceListType *e){

    // If the list or the head of the list is NULL, return immediately
    if(e == NULL || e->head == NULL)return;
    EvidenceNodeType * node = e->head;

    EvidenceNodeType * temp = NULL;

    while(node->next != NULL) // Traverse the list
    {
        temp = node->next; // Move the temp pointer to the next 
        free(node);
        node = temp;
    }
    free(node);
    e->size = 0; // Reset the size of the list to 0
    e->head = NULL;
}


/*
 Function: deleteEvidence

 Deletes a specific evidence from an EvidenceListType.

 Parameters:
   in/out list - Pointer to the EvidenceListType from which evidence is to be deleted.
   in evidence - The evidence data to be deleted.
   in/out node - used to loop through the linked list
   out temp - used to store the previous node in the linked list from node

 Returns: 
   (void) - Nothing is returned; it's a void function.
*/


// Function to delete a specific evidence node from the list
void deleteEvidence(EvidenceListType * list, EvidenceType evidence) {
    
    // If the list or the head of the list is NULL, return immediately
    if(list == NULL || list->head == NULL)return;

    EvidenceNodeType *node = list->head;
    EvidenceNodeType *temp = NULL;


    // If the evidence in the head of the list matches the evidence to be deleted
    if(list->head->evidence == evidence){
        node = list->head;
        list->head = list->head->next;

        free(node);
        return;
    }

    // Traverse the list until a node with the specified evidence is found
    while (node != NULL && node->evidence != evidence) {
        temp = node;
        node = node->next;
    }
    // if no mode with the specified evidence is found, return it
    if (node == NULL) return;

    temp->next = node->next;

    free(node);
}