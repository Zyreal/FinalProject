#include "defs.h"

// Hunter contains the room data

// initialize hunter
void initHunter(HunterType** hunter, char* name, EvidenceType et, EvidenceType* ec, RoomType* room) {
    HunterType* newHunter = malloc(sizeof(HunterType));

    strcpy(newHunter->name, name);
    // change to van room
    newHunter->room = room;
    newHunter->evidenceType = et;
    // pointing to collection?
    newHunter->evidenceCollection = ec;
    newHunter->fear = 0;
    newHunter->boredom = 0;
    (*hunter) = newHunter;
}

// initializing the hunter array
void initHunterArray(HunterArrayType* hunterArray) {
    hunterArray->size = 0;
}

// chooses a random room to move the hunter
void moveHunterToRoom(HunterType* hunter) {
    int chooseRoom = randInt(1, (&hunter->room->rooms)->size);
    // printf("%d, %d", chooseRoom, (&hunter->room->rooms)->size);
    RoomNodeType *newRoom = (&hunter->room->rooms)->head;
    int i;
    while (chooseRoom > 1) {
        newRoom = newRoom->next;
        chooseRoom--;
    }
    // delete hunter from room array and add to new room
    for (i = 0; i < (&hunter->room->hunterArray)->size; i++) {
        if ((&hunter->room->hunterArray)->hunters[i]->evidenceType == hunter->evidenceType) {
            break;
        }
    }

    // shift all hunters to the left
    while (i < (&hunter->room->hunterArray)->size) {
        (&hunter->room->hunterArray)->hunters[i] = (&hunter->room->hunterArray)->hunters[i + 1];
        i++;
    }
    (&hunter->room->hunterArray)->size--;

    // add hunter to new room
    (&newRoom->room->hunterArray)->hunters[(&newRoom->room->hunterArray)->size] = hunter;
    (&newRoom->room->hunterArray)->size++;

    // // change room of hunter
    hunter->room = newRoom->room;
    l_hunterMove(hunter->name, newRoom->room->name);
}

// Checks if there is a hunter in the room
int hasHunterInRoom(RoomType* room) {
    return room->hunterArray.size > 0;
}

// checks if hunters have enough evidence to leave
// change parameter to shared evidence from house
bool reviewEvidence(HunterType* hunter) {
    int totalEvidence = 0;
    for (int i = 0; i < NUM_HUNTERS; i++) {
        // need to initialize array to start at ev_unknown
        if (hunter->evidenceCollection[i] != EV_UNKNOWN) {
            totalEvidence++;
        }
    }
    if (totalEvidence >= 3) {
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
        return true;
    }
    else {
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
        return false;
    }
}

// function to make hunter exit thread (update linked list probs)
// void hunterExits(struct Hunter* hunter) {
//     // Checking if the fear of the hunter is greater than or equal to FEAR_MAX
//         if (hunter->fear >= FEAR_MAX) {
//             printf("%s is too scared to continue.", hunter->name);
//             pthread_exit(NULL);
//         }

//         // Checking if the boredom of the hunter is greater than or equal to BOREDOM_MAX
//         if (hunter->boredom >= BOREDOM_MAX) {
//             printf("%s is too bored.", hunter->name);
//             pthread_exit(NULL);
//         }
// }

void hunterCollect(HunterType* hunter) {
    EvidenceNodeType *currEvidence = (&hunter->room->evidences)->head;
    
    // While the list has contents, check to see if the evidence matches
    // If the evidence matches, collect the hunter's name, evidence, and room name
    while (currEvidence != NULL) {
        printf("has %d, for %d", hunter->evidenceType, currEvidence->evidenceType);
        if (hunter->evidenceType == currEvidence->evidenceType) {
            hunter->evidenceCollection[hunter->evidenceType] = hunter->evidenceType;
            l_hunterCollect(hunter->name, hunter->evidenceType, hunter->room->name);
            return;
        }

        // Moving to the next evidence node
        currEvidence = currEvidence->next;

    }

    // display something else for being unable to find evidence
    // l_hunterCollect(hunter->name, EV_UNKNOWN, hunter->room->name);
}

// Prints the names of the hunters in the list along with their boredom and fear level
void printHunters(HunterType* hunters[NUM_HUNTERS]) {
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        printf("Hunter %d: %s | Fear: %d | Boredom: %d\n", i + 1, hunters[i]->name, hunters[i]->fear, hunters[i]->boredom);
    }
}

void increaseDebuff(HunterType* hunter) {
    if (hunter->room->ghost != NULL) {
        hunter->fear++;
    }
    else {
        hunter->boredom++;
    }
}