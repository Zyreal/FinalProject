#include "defs.h"

// Ghost contains room data

void initGhost(GhostClass ghostType, RoomType* room, GhostType** gt) {
    GhostType* newGhost = malloc(sizeof(GhostType));
    newGhost->ghostType = ghostType;
    newGhost->room = room;
    newGhost->boredom = 0;
    (*gt) = newGhost;
}


// void* ghostBehaviour(void* arg) {

//     GhostType* ghost = (GhostType*)arg;

//     while (ghost->boredom < BOREDOM_MAX){
//         if (ghost->room->hunters.size > 0){
//             ghost->boredom = 0;
//             int action = randInt(0, 2);

//             if (action == 0) {
//                 enum EvidenceType evidenceType = randomGhostEvidence();
//                 l_ghostEvidence(evidenceType, ghost->room->name);
//                 addEvidenceToRoom(&ghost->room->evidences, evidenceType);
//             }
            
//         } else {
//             ghost->boredom++;

//             int action = randInt(0, 3);

//             if (action == 0) {
//                 moveGhostToAdjacentRoom(ghost);
//             } else if (action == 1) {
//                 enum EvidenceType evidenceType = randomGhostEvidence();
//                 l_ghostEvidence(evidenceType, ghost->room->name);
//                 addEvidenceToRoom(&ghost->room->evidences, evidenceType);
//             }
//             // For action == 2, don't do anything
//         }

//         usleep(GHOST_WAIT * 1000);
//     }

//     // Exiting the thread
//     pthread_exit(NULL);
// }

// random evidence is before function called, maybe just pass in ghost
void leaveEvidence(RoomType* room, enum EvidenceType evidenceType){
    EvidenceNodeType* newEvidenceNode = (EvidenceNodeType*)malloc(sizeof(EvidenceNodeType));
    newEvidenceNode->evidenceType = evidenceType;
    newEvidenceNode->next = NULL;

    if(room->evidences.head == NULL) {
        room->evidences.head = newEvidenceNode;
        room->evidences.tail = newEvidenceNode;
    } else {
        room->evidences.tail->next = newEvidenceNode;
        room->evidences.tail = newEvidenceNode;
    }

    room->evidences.size++;
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

    // change pointer of ghost in rooms
    ghost->room->ghost = NULL;
    newRoom->room->ghost = ghost;

    ghost->room = newRoom->room;
    l_ghostMove(newRoom->room->name);
}

// void ghostExits(struct Ghost* ghost) {
//     // If the ghost's boredom is too high, exit the thread
//     if (ghost->boredom >= BOREDOM_MAX) {
//         printf("%s", LOG_BORED);
//         pthread_exit(NULL);
//     }
// }

const char* ghostClassToString(enum GhostClass ghostType) {
    switch (ghostType) {
        case POLTERGEIST:
            return "Poltergeist";
        case BANSHEE:
            return "Banshee";
        case BULLIES:
            return "Bullies";
        case PHANTOM:
            return "Phantom";
        case GH_UNKNOWN:
        default:
            return "Unknown Ghost";
    }
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
