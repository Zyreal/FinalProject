#include "defs.h"

// initialize room
RoomType* createRoom(char* roomName) {
    RoomType* newRoom = (RoomType*) malloc(sizeof(RoomType));
    strcpy(newRoom->name, roomName);
    initRoomList(&newRoom->rooms);
    initEvidenceList(&newRoom->evidences);
    initHunterArray(&newRoom->hunterArray);
    newRoom->ghost = NULL;
    sem_init(&newRoom->mutex, 0, 1);
    return newRoom;
}


// initialize room list
void initRoomList(RoomListType* rl) {
    // may need to malloc
    rl->size = 0;
    rl->head = NULL;
    rl->tail = NULL;
}


// connect 2 rooms together
void connectRooms(RoomType* room1, RoomType* room2) {

    // connecting room1 to room2
    addRoom((&room1->rooms), room2);

    // connecting room2 to room1
    addRoom((&room2->rooms), room1);
}


int isRoomConnected(RoomListType roomList){
    RoomNodeType* currRoom = (&roomList)->head;

    while (currRoom != NULL){
        printf("%s ", currRoom->room->name);
        currRoom = currRoom->next;
    }

    return 0;
}


// connects 1 room to the room list of another room
void addRoom(RoomListType* rl, RoomType* room) {
    RoomNodeType* newNode = (RoomNodeType*) malloc(sizeof(RoomNodeType));

    newNode->room = room;
    newNode->next = NULL;

    if ((rl)->head == NULL) {
        (rl)->head = newNode;
        (rl)->tail = newNode;
    }
    else {
        (rl)->tail->next = newNode;
        (rl)->tail = newNode;
    }
    rl->size++;
}

// Cleaning up the rooms
void cleanupRoom(RoomType* room) {
    cleanupEvidence(&room->evidences);
    RoomNodeType *head = (&room->rooms)->head;
    RoomNodeType *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Cleaning the list of rooms
void cleanupRoomList(RoomListType *rl) {
    RoomNodeType *head = rl->head;
    RoomNodeType *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        cleanupRoom(temp->room);
        free(temp->room);
        free(temp);
    }
}