#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
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
// typedef struct Evidence     EvidenceType;
typedef struct EvidenceNode EvidenceNodeType;
typedef struct Hunter       HunterType;
typedef struct HunterArray  HunterArrayType;
typedef struct Ghost        GhostType;
typedef struct House        HouseType;

const char* ghostClassToString(enum GhostClass ghostType);

struct HunterArray {
    HunterType* hunters[NUM_HUNTERS];
    int size;
};

// Hunter struct
struct Hunter {
    char name[MAX_STR];
    RoomType* room;
    EvidenceType evidenceType;
    int *evidenceCollection;
    int fear;
    int boredom;
};

struct RoomList {
    RoomNodeType* head;
    RoomNodeType* tail;
    int size;
};

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

// Room struct
struct Room {
    char name[MAX_STR];
    RoomListType rooms;
    EvidenceListType evidences;
    HunterArrayType hunterArray;
    GhostType* ghost;
    // struct Ghost* ghosts;
    // struct Hunter* hunters;
};

// Ghost struct
struct Ghost {
    enum GhostClass ghostType;
    RoomType *room;
    int boredom;
    // struct Ghost* next;
};

struct Node {
  struct Room *room;
  struct Node *next;
};

struct House{
    HunterArrayType hunterArray;
    RoomListType rooms;
    int *evidenceCollection;
};

void initHouse(HouseType *house, EvidenceListType *el);
void populateRooms(HouseType* house);

// Initializing rooms
RoomType* createRoom(char* roomName);
void initRoomList(RoomListType *rl);

// Connecting and adding rooms
void connectRooms(RoomType* room1, RoomType* room2);
void addRoom(RoomListType* rl, RoomType* room);

// Ghost behaviours
void* ghostBehaviour(void* arg);
void ghostMove(GhostType* ghost);

int hasHunterInRoom(RoomType* room);
void leaveEvidence(RoomType* room, enum EvidenceType evidenceType);



// Function to initialize hunter
void initHunter(HunterType** hunter, char* name, EvidenceType et, int* ec, RoomType* room);
void initHunterArray(HunterArrayType* hunterArray);

// Function to add a hunter to a room
void moveHunterToRoom(HunterType* hunter);

// Reviewing hunter evidence
void reviewEvidence(HunterType* hunter);

// Exiting a thread if the hunter's fear or boredom is too high
// void hunterExits(struct Hunter* hunter);


void initGhost(GhostClass ghostType, RoomType* room, GhostType** gt);

// Exiting a thread if the ghost's boredom is too high
void ghostExits(struct Ghost* ghost);

// Allows the hunter to collect evidence
void hunterCollect(HunterType* hunter);


void printHunters(HunterType *hunters[NUM_HUNTERS]);
void printGhosts(GhostType* ghost, int numGhosts);
void printMenu(int *choice);

void deleteRoom(RoomType* room);

void initEvidenceList(EvidenceListType* el);
int isRoomConnected(RoomListType roomList);



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

int isRoomConnected(RoomListType roomList);