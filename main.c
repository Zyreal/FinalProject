#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));
    pthread_t g, h1, h2, h3, h4;
    /* Note: You may change it, but it's here for demonstration purposes
        This code will not compile until you have implemented the house functions and structures
        HouseType house;
        initHouse(&house);
        populateRooms(&house);
    */

    EvidenceType el[] = {EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN};
    // TESTING ROOMS
    HouseType house;
    initHouse(&house, el);
    populateRooms(&house);



    // TESTING GHOSTS
    GhostType* ghost;
    int rand = randInt(0, (&(&house)->rooms)->size);
    RoomNodeType *n = (&(&house)->rooms)->head;
    while (rand > 0) {
        n = n->next;
        rand--;
    }
    initGhost(randomGhost(), n->room, &ghost);
    l_ghostInit(ghost->ghostType, n->room->name);
    // pthread_create(&g, NULL, ghostBehaviour, ghost);
    // pthread_join(g, NULL);

    // initGhost(randomGhost(), (&(&house)->rooms)->head->next->room, &ghost);
    // l_ghostInit(ghost->ghostType, (&(&house)->rooms)->head->next->room->name);

    // if (pthread_create(&g, NULL, ghostbehaviour(), ghost) != 0){
    //     fprintf(stderr, "Error creating ghost thread");
    //     return 1;
    // }


    // TESTING HUNTERS
    char hunterName[MAX_STR];
    HunterType* hunters[NUM_HUNTERS];
    
    for (int i = 0; i < NUM_HUNTERS; i++){
        printf("\nEnter name of Hunter %d: ", i+1);
        scanf("%s", hunterName);
        l_hunterInit(hunterName, i);
        initHunter(&hunters[i], hunterName, i, (&house)->evidenceCollection, (&(&house)->rooms)->head->room); 
    }
    // printf("\n");
    // printHunters(hunters);

    // moveHunterToRoom(hunters[0]);
    // printf("%s\n",hunters[0]->room->name);

    // moveHunterToRoom(hunters[1]);
    // printf("%s\n",hunters[1]->room->name);
        
    // moveHunterToRoom(hunters[2]);
    // printf("%s\n",hunters[2]->room->name);
        
    // moveHunterToRoom(hunters[3]);
    // printf("%s\n",hunters[3]->room->name);
  
    // leaveEvidence(ghost->room, 0);
    // ghostMove(ghost);

    // for (int z = 0; z < NUM_HUNTERS; z++) {
    //     hunterCollect(hunters[z]);
    //     printf("\n");
    // }

    // // testing if evidence is shared
    // for(int q = 0; q < NUM_HUNTERS; q++) {
    //     printf("%d\nhouse: %d\n",q, house.evidenceCollection[q]);
    //     printf("hunter 1: %d\n",hunters[0]->evidenceCollection[q]);
    //     printf("hunter 2: %d\n",hunters[1]->evidenceCollection[q]);
    //     printf("hunter 3: %d\n",hunters[2]->evidenceCollection[q]);
    //     printf("hunter 4: %d\n",hunters[3]->evidenceCollection[q]);
    // }

    pthread_create(&h1, NULL, hunterBehaviour, hunters[0]);
    pthread_join(h1, NULL);

    return 0;
}


void* ghostBehaviour(void* arg) {

    GhostType* ghost = (GhostType*)arg;
    int action;

    while (ghost->boredom < BOREDOM_MAX){
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
    // if the hunter's boredom is above 100, they will exit
    if (hunter->boredom > BOREDOM_MAX) {
        printf("The Ghost has won!\n\n");
        l_hunterExit(hunter->name, LOG_BORED);
    }
    // if the hunter's fear is above 100, they will exit
    else if (hunter->fear > FEAR_MAX) {
        printf("The Ghost has won!\n\n");
        l_hunterExit(hunter->name, LOG_FEAR);
    }

    // Exiting the thread
    pthread_exit(NULL);
}
