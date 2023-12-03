#include "defs.h"

/*
  Function: Create Room
  Purpose:  Initializes a RoomType
  Params:
      Input: char* roomName - name of the RoomType
  Return: RoomType* - pointer of newly initialized RoomType
*/
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


/*
  Function: Initialize RoomList
  Purpose:  Initializes a RoomList
  Params:
      Output: RoomListType* rl - RoomListType to be initialized
*/
void initRoomList(RoomListType* rl) {
    rl->size = 0;
    rl->head = NULL;
    rl->tail = NULL;
}


/*
  Function: Connect Rooms
  Purpose:  Connects 2 rooms together by their RoomList
  Params:
      Input: RoomType* room1 - first room to be connected
      Input: RoomType* room2 - second room to be connected
*/
void connectRooms(RoomType* room1, RoomType* room2) {

    // connecting room1 to room2
    addRoom((&room1->rooms), room2);

    // connecting room2 to room1
    addRoom((&room2->rooms), room1);
}

/*
  Function: Add Room
  Purpose:  Adds a RoomType to a RoomListType
  Params:
      Input: RoomType* room - RoomType to be added
      Output: RoomListType* rl - RoomListType to store RoomType
*/
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

/*
  Function: Cleanup Room
  Purpose:  Frees all allocated data of a RoomType
  Params:
      Input/Output: RoomType* room - RoomType to be freed
*/
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

/*
  Function: Cleanup RoomList
  Purpose:  Frees all allocated data of a RoomListType
  Params:
      Input/Output: RoomListType* rl - RoomListType to be freed
*/
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