#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));
    pthread_t g, h1, h2, h3, h4;

    // create collection to store evidences
    EvidenceType el[] = {EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN};
    
    // Initialize HOUSE and ROOMS
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
    // Asking the user to enter the names of 4 hunters
    for (int i = 0; i < NUM_HUNTERS; i++){
        printf("\nEnter name of Hunter %d: ", i+1);
        scanf("%s", hunterName);
        l_hunterInit(hunterName, i);
        initHunter(&(&(&house)->hunterArray)->hunters[i], hunterName, i, (&house)->evidenceCollection, (&(&house)->rooms)->head->room); 
        (&(&house)->hunterArray)->size++;
    }


    // Creating a ghost thread, and 4 hunter threads
    pthread_create(&g, NULL, ghostBehaviour, ghost);
    pthread_create(&h1, NULL, hunterBehaviour, (&(&house)->hunterArray)->hunters[0]);
    pthread_create(&h2, NULL, hunterBehaviour, (&(&house)->hunterArray)->hunters[1]);
    pthread_create(&h3, NULL, hunterBehaviour, (&(&house)->hunterArray)->hunters[2]);
    pthread_create(&h4, NULL, hunterBehaviour, (&(&house)->hunterArray)->hunters[3]);


    // join threads back
    pthread_join(g, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);


    // print results of ghost hunt
    printResults(&house, ghost);


    // Cleaning up the house with the hunters and ghost
    cleanupHouse(&house);
    free(ghost);

    return 0;
}

/*
  Function: Print Results
  Purpose:  Prints final results of program
  Params:
      Input: HouseType* house - HouseType to check its hunters
      Input: GhostType* ghost - GhostType to check its fields
*/
void printResults(HouseType* house, GhostType* ghost) {
    printf("========================================\n");
    printf("All done. Let's tally the results...\n");
    printf("========================================\n");

        for (int i = 0; i < (&(house)->hunterArray)->size; i++) {
            checkHunterStatus((&(house)->hunterArray)->hunters[i]);
        }
    printf("----------------------------------------\n");

    // check if hunters have correct amount of evidence
    int evidenceNum = 0;
    for (int j = 0; j < NUM_HUNTERS; j++) {
        if (house->evidenceCollection[j] != EV_UNKNOWN) {
            evidenceNum++;
        }
    }

    char ghost_str[MAX_STR];
    if (evidenceNum >= 3) {
        printf("It seems the ghost has been discovered!\n");
        printf("The hunters have won the game!\n");
        ghostToString(ghost->ghostType, ghost_str);


        printf("Using the evidence they found, they correctly determined that the ghost is a %s\n", ghost_str);
    }
    else {
        printf("The hunters failed!\n");
        ghostToString(GH_UNKNOWN, ghost_str);
        printf("Using the evidence they found, they incorrectly determined that the ghost is a %s\n", ghost_str);
        ghostToString(ghost->ghostType, ghost_str);
        printf("The ghost is actually a %s\n", ghost_str);
    }


    printf("The hunters collected the following evidence\n");
    for (int j = 0; j < NUM_HUNTERS; j++) {
        if (house->evidenceCollection[j] != EV_UNKNOWN) {
            char ev_str[MAX_STR];
            evidenceToString(house->evidenceCollection[j], ev_str);
            printf("    *%s\n", ev_str);
        }
    }
}