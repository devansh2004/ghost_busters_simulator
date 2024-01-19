#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR 64
#define MAX_RUNS 50
#define BOREDOM_MAX 100
#define C_TRUE 1
#define C_FALSE 0
#define HUNTER_WAIT 5000
#define GHOST_WAIT 600
#define NUM_HUNTERS 4
#define FEAR_MAX 10
#define LOGGING C_TRUE


typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType
{
    EMF,
    TEMPERATURE,
    FINGERPRINTS,
    SOUND,
    EV_COUNT,
    EV_UNKNOWN
};
enum GhostClass
{
    POLTERGEIST,
    BANSHEE,
    BULLIES,
    PHANTOM,
    GHOST_COUNT,
    GH_UNKNOWN
};
enum LoggerDetails
{
    LOG_FEAR,
    LOG_BORED,
    LOG_EVIDENCE,
    LOG_SUFFICIENT,
    LOG_INSUFFICIENT,
    LOG_UNKNOWN
};



// Helper Utilies
int randInt(int, int);                            // Pseudo-random number generator function
float randFloat(float, float);                    // Pseudo-random float generator function
enum GhostClass randomGhost();                    // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char *);      // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char *); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char *name, enum EvidenceType equipment);
void l_hunterMove(char *name, char *room);
void l_hunterReview(char *name, enum LoggerDetails reviewResult);
void l_hunterCollect(char *name, enum EvidenceType evidence, char *room);
void l_hunterExit(char *name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char *room);
void l_ghostMove(char *room);
void l_ghostEvidence(enum EvidenceType evidence, char *room);
void l_ghostExit(enum LoggerDetails reason);

/* These rename the structures that we'll be creating. It's an alternative to the format we've seen previously. */
typedef struct House HouseType;
typedef struct Ghost GhostType;
typedef struct Room RoomType;
typedef struct RoomNode RoomNodeType;
typedef struct RoomList RoomListType;
typedef struct Hunter HunterType;
typedef struct Evidence Evidence;
typedef struct EvidenceNode EvidenceNodeType;
typedef struct EvidenceList EvidenceListType;

struct Ghost
{
    enum GhostClass class;
    RoomType *currentRoom;
    int boredom;
};

struct RoomNode
{

    RoomType *room;
    RoomNodeType *next;
};

struct RoomList
{
    sem_t mutex;
    RoomNodeType *head;
    int size;
};


struct EvidenceNode
{
    EvidenceType evidence;
    EvidenceNodeType *next;
};

struct EvidenceList
{
    sem_t mutex;
    int size;
    EvidenceNodeType *head;
};

/*
struct Evidince
{
    Evidence *type;
    float value;
};
*/




struct Room
{

    RoomListType connectedRooms;
    char name[MAX_STR];
    EvidenceListType evidenceList;
    HunterType *huntersInRoom[NUM_HUNTERS];
    GhostType * ghost;
};

struct Hunter
{
    char name[MAX_STR];
    RoomType *currentRoom;
    EvidenceType equipment;
    EvidenceListType *evidenceList;
    int fear;
    int boredom; 
};

struct House
{
    GhostType *ghost;
    RoomListType rooms;
    HunterType *hunters[NUM_HUNTERS];
    EvidenceListType evidenceList;
};

// Function Prototypes

void *hunterThread(void *arg);
void *ghostThread(void *arg);

// Hunter Functions:
void initHunter(HunterType **h, enum EvidenceType evidenceType, RoomType *room, EvidenceListType * evidenceList,int *taken);

// Ghost Functions:

void initGhost(GhostType **g, enum GhostClass ghostClass, RoomType *room);

// Room Functions:

void initRoomList(RoomListType *list);
RoomType *createRoom(char *name);
void connectRooms(RoomType *r1, RoomType *r2);
void addRoom(RoomListType *List, RoomType *room);
void cleanUpRoomData(RoomListType *r);
void cleanUpRoom(RoomListType *r);

// House Functions:
void populateRooms(HouseType *house);
void initHouse(HouseType *h);
void cleanUpHouse(HouseType *h);

//Evidence Function
void initEvidenceList(EvidenceListType *list);
void addEvidence(EvidenceListType *list , EvidenceType evidence);
void cleanUpEvidence(EvidenceListType *e);
void deleteEvidence(EvidenceListType * list, EvidenceType evidence);