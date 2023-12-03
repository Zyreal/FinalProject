#include "defs.h"

/*
  Function: Initialize Hunter
  Purpose:  Initializes all fields of a HunterType
  Params:
      Input: char* name - Name of the HunterType
      Input: EvidenceType - EvidenceType that the HunterType can collect
      Input: EvidenceType* ec - collection of EvidenceType that is shared with the HunterType
      Input/Output: RoomType* room - RoomType that the hunter starts in
      Input/Output: HunterType** hunter - HunterType to be initialized
*/
void initHunter(HunterType** hunter, char* name, EvidenceType et, EvidenceType* ec, RoomType* room) {
    HunterType* newHunter = malloc(sizeof(HunterType));

    strcpy(newHunter->name, name);
    // change to van room
    newHunter->room = room;
    (&newHunter->room->hunterArray)->hunters[(&newHunter->room->hunterArray)->size] = newHunter;
    (&newHunter->room->hunterArray)->size++;
    // printf("%d", (&newHunter->room->hunterArray)->size);

    newHunter->evidenceType = et;
    newHunter->evidenceCollection = ec;
    newHunter->fear = 0;
    newHunter->boredom = 0;
    (*hunter) = newHunter;
}

/*
  Function: Initialize Hunter Array
  Purpose:  Initializes all fields of a HunterArrayType
  Params:
      Input/Output: HunterArrayType* hunterArray - HunterArrayType to be initialized
*/
void initHunterArray(HunterArrayType* hunterArray) {
    hunterArray->size = 0;
}

/*
  Function: Move Hunter To Room
  Purpose:  Moves HunterType to a connect RoomType
  Params:
      Input/Output: HunterType* hunter - HunterType to be moved
*/
void moveHunterToRoom(HunterType* hunter) {
    int chooseRoom = randInt(1, (&hunter->room->rooms)->size);
    // printf("%d, %d", chooseRoom, (&hunter->room->rooms)->size);
    RoomNodeType *newRoom = (&hunter->room->rooms)->head;
    while (chooseRoom > 1) {
        newRoom = newRoom->next;
        chooseRoom--;
    }
    sem_wait(&hunter->room->mutex);
    sem_wait(&newRoom->room->mutex);
    
    removeHunter(hunter);

    // add hunter to new room
    (&newRoom->room->hunterArray)->hunters[(&newRoom->room->hunterArray)->size] = hunter;
    (&newRoom->room->hunterArray)->size++;
    
    sem_post(&hunter->room->mutex);
    sem_post(&newRoom->room->mutex);

    // change room of hunter
    hunter->room = newRoom->room;
    l_hunterMove(hunter->name, newRoom->room->name);
}

/*
  Function: Remove Hunter
  Purpose:  Removes HunterType from the HunterArrayType of a RoomType
  Params:
      Input/Output: HunterType* hunter - HunterType to be removed
*/
void removeHunter(HunterType* hunter) {
    int i;
    
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
}



/*
  Function: Review Evidence
  Purpose:  Checks if HunterType has sufficient EvidenceType to leave
  Params:
      Input: HunterType* hunter - HunterType to check EvidenceType
  Return: true if HunterType has sufficient EvidenceType, false otherwise
*/
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

/*
  Function: Hunter Collect
  Purpose:  Attempts to collect EvidenceType from a RoomType
  Params:
      Input/Output: HunterType* hunter - HunterType that will collect EvidenceType
*/
void hunterCollect(HunterType* hunter) {

    sem_wait(&hunter->room->mutex);
    EvidenceNodeType *currEvidence = (&hunter->room->evidences)->head;
    EvidenceNodeType *prev = NULL;
    // While the list has contents, check to see if the evidence matches
    // If the evidence matches, collect the hunter's name, evidence, and room name
    while (currEvidence != NULL) {
        if (hunter->evidenceType == currEvidence->evidenceType) {
            hunter->evidenceCollection[hunter->evidenceType] = hunter->evidenceType;
            l_hunterCollect(hunter->name, hunter->evidenceType, hunter->room->name);
            if (prev == NULL) {
                currEvidence = NULL;
            }
            else {
                prev->next = currEvidence->next;
                free(currEvidence);
            }
            break;
        }
        prev = currEvidence;
        // Moving to the next evidence node
        currEvidence = currEvidence->next;
    }
    sem_post(&hunter->room->mutex);
}

/*
  Function: Increase Debuff
  Purpose:  Increase fear or boredom of HunterType
  Params:
      Input/Output: HunterType* hunter - HunterType to be targeted
*/
void increaseDebuff(HunterType* hunter) {
    if (hunter->room->ghost != NULL) {
        hunter->fear++;
    }
    else {
        hunter->boredom++;
    }
}

/*
  Function: Check Hunter Status
  Purpose:  Checks if HunterType fear or boredom has reached max
  Params:
      Input: HunterType* hunter - HunterType to be checked
*/
void checkHunterStatus(HunterType* hunter) {
    if (hunter->boredom > BOREDOM_MAX) {
        printf("        *%s has gotten too bored to continue!\n", hunter->name);
    }
    else if (hunter->fear > FEAR_MAX) {
        printf("        *%s has run away in fear!\n", hunter->name);
    }
}

/*
  Function: Cleanup Hunter
  Purpose:  Frees all allocated data of a HunterType
  Params:
      Output: HunterType* hunter - HunterType to be freed
*/
void cleanupHunter(HunterType* hunter) {
    free(hunter);
}

/*
  Function: Cleanup Hunter Array
  Purpose:  Frees all allocated data of a HunterArrayType
  Params:
      Input: HunterArrayType* hunterArray - HunterArrayType to be freed
*/
void cleanupHunterArray(HunterArrayType* hunterArray) {
    int i;
    for (i = 0; i < hunterArray->size; i++) {
        cleanupHunter(hunterArray->hunters[i]);
    }
}

/*
  Function: Hunter Behaviour
  Purpose:  Loops through all HunterType actions until their fear or boredom reaches max
  Params:
      Input: void* arg -  HunterType that is chosen
  Return: Nothing
*/
void* hunterBehaviour(void* arg) {
    HunterType* hunter = (HunterType*) arg;
    int action;
    while (hunter->boredom <= BOREDOM_MAX && hunter->fear <= FEAR_MAX ) {
        increaseDebuff(hunter);
        action = randInt(0, 3);
        if (action == 0) {
            moveHunterToRoom(hunter);
        }
        else if (action == 1) {
            hunterCollect(hunter);
        }
        else if (action == 2) {
            bool hasEvidence = reviewEvidence(hunter);
            if (hasEvidence == true) {
                l_hunterExit(hunter->name, LOG_EVIDENCE);
                break;
            }
        }
        usleep(HUNTER_WAIT);
    }
    removeHunter(hunter);

    // if the hunter's boredom is above 100, they will exit
    if (hunter->boredom >=BOREDOM_MAX) {
        l_hunterExit(hunter->name, LOG_BORED);
    }
    // if the hunter's fear is above 100, they will exit
    else if (hunter->fear >= FEAR_MAX) {
        l_hunterExit(hunter->name, LOG_FEAR);
    }
    
    // Exiting the thread
    pthread_exit(NULL);
}
