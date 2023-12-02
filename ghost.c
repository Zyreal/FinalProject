#include "defs.h"


void initGhost(GhostClass ghostType, RoomType* room, GhostType** gt) {
    GhostType* newGhost = malloc(sizeof(GhostType));
    newGhost->ghostType = ghostType;
    newGhost->room = room;
    newGhost->boredom = 0;
    (*gt) = newGhost;
}


// random evidence is before function called, maybe just pass in ghost
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

// void printGhosts(GhostType* ghosts, int numGhosts) {
//     printf("\nGHOSTS:\n");

//     for (int i = 0; i < numGhosts; ++i){
//         printf("Ghost %d:\n", i + 1);
//         printf("    Ghost Type: %s\n", ghostClassToString(ghosts[i].ghostType));
        
//         if (ghosts[i].room != NULL){
//             printf("    Room: %s\n", ghosts[i].room->name);
//         } else {
//             printf("    Room: (null)\n");
//         }

//         printf("    Boredom: %d\n", ghosts[i].boredom);
//         printf("\n");
//     }
// }

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