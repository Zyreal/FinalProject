#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE
#define MAX_ARR         128

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

// Structure Definitions
typedef struct RoomList     RoomListType;
typedef struct Room         RoomType;
typedef struct RoomNode     RoomNodeType;
typedef struct EvidenceList EvidenceListType;
typedef struct EvidenceNode EvidenceNodeType;
typedef struct Hunter       HunterType;
typedef struct HunterArray  HunterArrayType;
typedef struct Ghost        GhostType;
typedef struct House        HouseType;

const char* ghostClassToString(enum GhostClass ghostType);

// Array of hunters
struct HunterArray {
    HunterType* hunters[NUM_HUNTERS];
    int size;
};

// Hunter struct:  contains a name, room data, type of evidence, and a collection of evidence, fear, and boredom
struct Hunter {
    char name[MAX_STR];
    RoomType* room;
    EvidenceType evidenceType;
    EvidenceType *evidenceCollection;
    int fear;
    int boredom;
    sem_t mutex;
};

// Linked list of rooms
struct RoomList {
    RoomNodeType* head;
    RoomNodeType* tail;
    int size;
};

// Each room node 
struct RoomNode {
    RoomType *room;
    RoomNodeType *next;
};

struct EvidenceList {
    EvidenceNodeType *head;
    EvidenceNodeType *tail;
    int size;
};

struct EvidenceNode {
    enum EvidenceType evidenceType;
    struct EvidenceNode *next;
};

// Room struct: contains a ghost, a collection of evidence, and an array of Hunters
struct Room {
    char name[MAX_STR];
    RoomListType rooms;
    EvidenceListType evidences;
    HunterArrayType hunterArray;
    GhostType* ghost;
    sem_t mutex;
};

// Ghost struct
struct Ghost {
    enum GhostClass ghostType;
    RoomType *room;
    int boredom;
};

// Room node: contains pointer to the next node
struct Node {
  struct Room *room;
  struct Node *next;
};

// House struct: contains rooms, an array of Hunters, and a collection of evidence
struct House{
    HunterArrayType hunterArray;
    RoomListType rooms;
    EvidenceType *evidenceCollection;
};


// HOUSE AND ROOM FUNCTIONS
void initHouse(HouseType *house, EvidenceType *el);
void initEvidenceList(EvidenceListType* el);
void cleanupEvidence(EvidenceListType* el);
void populateRooms(HouseType* house);
RoomType* createRoom(char* roomName);
void initRoomList(RoomListType *rl);
void connectRooms(RoomType* room1, RoomType* room2);
void addRoom(RoomListType* rl, RoomType* room);
void cleanupHouse(HouseType* house);


// GHOST FUNCTIONS
void initGhost(GhostClass ghostType, RoomType* room, GhostType** gt);
void* ghostBehaviour(void* arg);
void ghostMove(GhostType* ghost);
void ghostExits(struct Ghost* ghost);
EvidenceType randomGhostEvidence(GhostType* ghost);
void printGhosts(GhostType* ghost, int numGhosts);


// HUNTER FUNCTIONS
void initHunter(HunterType** hunter, char* name, EvidenceType et, EvidenceType* ec, RoomType* room);
void* hunterBehaviour(void* arg);
void initHunterArray(HunterArrayType* hunterArray);
void moveHunterToRoom(HunterType* hunter);
void removeHunter(HunterType* hunter);
bool reviewEvidence(HunterType* hunter);
void hunterCollect(HunterType* hunter);
void printHunters(HunterType* hunters[NUM_HUNTERS]);
void increaseDebuff(HunterType* hunter);
void cleanupHunterArray(HunterArrayType* hunterArray);
void cleanupHunter(HunterType* hunter);


// ROOM FUNCTIONS
int isRoomConnected(RoomListType roomList); // delete
int hasHunterInRoom(RoomType* room);
void leaveEvidence(RoomType* room, enum EvidenceType evidenceType);
void cleanupRoom(RoomType* room);
void cleanupRoomList(RoomListType *rl);


// Helper Utilities
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);