#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));
    pthread_t g, h1, h2, h3, h4;

    // create collection to store evidences
    EvidenceType el[] = {EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN};
    
    // Initialize ROOMS
    HouseType house;
    initHouse(&house, el);
    populateRooms(&house);



    // Initialize GHOSTS
    GhostType* ghost;
    int rand = randInt(0, (&(&house)->rooms)->size);
    RoomNodeType *n = (&(&house)->rooms)->head;
    while (rand > 0) {
        n = n->next;
        rand--;
    }
    initGhost(randomGhost(), n->room, &ghost);
    l_ghostInit(ghost->ghostType, n->room->name);

    // Initialize HUNTERS
    char hunterName[MAX_STR];
    HunterType* hunters[NUM_HUNTERS];
    
    // Asking the user to enter the names of four hunters
    for (int i = 0; i < NUM_HUNTERS; i++){
        printf("\nEnter name of Hunter %d: ", i+1);
        scanf("%s", hunterName);
        l_hunterInit(hunterName, i);
        initHunter(house->hunterArray->hunters[i], hunterName, i, (&house)->evidenceCollection, (&(&house)->rooms)->head->room); 
    }
    
    // Creating a ghost thread, and 4 hunter threads
    pthread_create(&g, NULL, ghostBehaviour, ghost);
    pthread_create(&h1, NULL, hunterBehaviour, hunters[0]);
    pthread_create(&h2, NULL, hunterBehaviour, hunters[1]);
    pthread_create(&h3, NULL, hunterBehaviour, hunters[2]);
    pthread_create(&h4, NULL, hunterBehaviour, hunters[3]);

    pthread_join(g, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);

    // Cleaning up the house and freeing the 4 hunters
    cleanupHouse(&house);
    free(hunters[0]);
    free(hunters[1]);
    free(hunters[2]);
    free(hunters[3]);
    free(ghost);
    
    return 0;
}


void* ghostBehaviour(void* arg) {

    GhostType* ghost = (GhostType*)arg;
    int action;

    while (ghost->boredom < BOREDOM_MAX){
        printf("g bore %d in room: %s of %d hunters\n", ghost->boredom, ghost->room->name, (&ghost->room->hunterArray)->size);
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
    printf("now exiting thread");
    removeHunter(hunter);

    // if the hunter's boredom is above 100, they will exit
    if (hunter->boredom >=BOREDOM_MAX) {
        l_hunterExit(hunter->name, LOG_BORED);
        printf("\n\nThe Ghost has won!\n\n");
    }
    // if the hunter's fear is above 100, they will exit
    else if (hunter->fear >= FEAR_MAX) {
        l_hunterExit(hunter->name, LOG_FEAR);
        printf("\n\nThe Ghost has won!\n\n");
    }
    
    // Exiting the thread
    pthread_exit(NULL);
}
