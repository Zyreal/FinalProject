#include "defs.h"

/*
  Function: Initialize Ghost
  Purpose:  Frees all allocated data of a GhostType
  Params:
      Input: GhostClass ghostType - GhostClass of the GhostType
      Input: RoomType* room - RoomType that the ghost starts in
      Input/Output: GhostType** gt - GhostType to be initialized
*/
void initGhost(GhostClass ghostType, RoomType* room, GhostType** gt) {
    GhostType* newGhost = malloc(sizeof(GhostType));
    newGhost->ghostType = ghostType;
    newGhost->room = room;
    newGhost->boredom = 0;
    (*gt) = newGhost;
}


/*
  Function: Leave Evidence
  Purpose: Leaves evidence stored in a GhostType
  Params:
      Input: enum EvidenceType evidenceType - EvidenceType to be left
      Input/Output: RoomType* room - RoomType that will hold the EvidenceType
*/
void leaveEvidence(RoomType* room, enum EvidenceType evidenceType){
    EvidenceNodeType* newEvidenceNode = (EvidenceNodeType*)malloc(sizeof(EvidenceNodeType));
    newEvidenceNode->evidenceType = evidenceType;
    newEvidenceNode->next = NULL;

    sem_wait(&room->mutex);
    if(room->evidences.head == NULL) {
        room->evidences.head = newEvidenceNode;
        room->evidences.tail = newEvidenceNode;
    } else {
        room->evidences.tail->next = newEvidenceNode;
        room->evidences.tail = newEvidenceNode;
    }

    room->evidences.size++;
    sem_post(&room->mutex);
    l_ghostEvidence(evidenceType, room->name);
}

/*
  Function: Ghost Move
  Purpose:  Moves the ghost to another connected RoomType
  Params:
      Input/Output: GhostType* ghost - GhostType that will be moved
*/
void ghostMove(GhostType* ghost) {
    // ghost->room->rooms->size
    int newRoomIndex = randInt(1, (&ghost->room->rooms)->size);
    RoomNodeType *newRoom = (&ghost->room->rooms)->head;

    // loop through room list until random index is found
    for (int i = 1; i < newRoomIndex; i++) {
        newRoom = newRoom->next;
    }

    sem_wait(&ghost->room->mutex);
    // change pointer of ghost in rooms
    ghost->room->ghost = NULL;
    newRoom->room->ghost = ghost;
    sem_post(&ghost->room->mutex);

    ghost->room = newRoom->room;
    l_ghostMove(newRoom->room->name);
}

/*
  Function: Random Ghost Evidence
  Purpose:  Chooses a random EvidenceType based on the GhostType
  Params:
      Input: GhostType* ghost - GhostType whose evidence will be chosen
  Return: random EvidenceType based on the GhostType
*/
EvidenceType randomGhostEvidence(GhostType* ghost) {
    // magic num
    int i = randInt(0, 3);
    EvidenceType e[3];
    if (ghost->ghostType == POLTERGEIST) {
        e[0] = EMF;
        e[1] = TEMPERATURE;
        e[2] = FINGERPRINTS;
    }
    else if (ghost->ghostType == BANSHEE) {
        e[0] = EMF;
        e[1] = TEMPERATURE;
        e[2] = SOUND;
    }
    else if (ghost->ghostType == BULLIES) {
        e[0] = EMF;
        e[1] = FINGERPRINTS;
        e[2] = SOUND;
    }
    else if (ghost->ghostType == PHANTOM) {
        e[0] = TEMPERATURE;
        e[1] = FINGERPRINTS;
        e[2] = SOUND;
    }
    return e[i];

}

/*
  Function: Ghost Behaviour
  Purpose:  Loops through all GhostType actions until their boredom reaches max
  Params:
      Input: void* arg -  GhostType that is chosen
  Return: Nothing
*/
void* ghostBehaviour(void* arg) {

    GhostType* ghost = (GhostType*)arg;
    int action;

    while (ghost->boredom < BOREDOM_MAX){
        // printf("g bore %d in room: %s of %d hunters\n", ghost->boredom, ghost->room->name, (&ghost->room->hunterArray)->size);
        if (ghost->room->hunterArray.size > 0){
            // generate a random action for ghosts when hunters are present
            action = randInt(0, 2);
            ghost->boredom = 0;
        } else {
            // generate a random action for ghosts when no hunters are present
            action = randInt(0, 3);
            ghost->boredom++;
        }
        
        if (action == 1) {
            EvidenceType evidenceType = randomGhostEvidence(ghost);
            // printf("leaving %d\n", evidenceType);
            leaveEvidence(ghost->room, evidenceType);
        } else if (action == 2) {
            ghostMove(ghost);
        } 

        usleep(GHOST_WAIT);
    }

    l_ghostExit(LOG_BORED);
    
    // Exiting the thread
    pthread_exit(NULL);
}