#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    
    /* Note: You may change it, but it's here for demonstration purposes
        This code will not compile until you have implemented the house functions and structures
        HouseType house;
        initHouse(&house);
        populateRooms(&house);
    */

    EvidenceListType el[NUM_HUNTERS];
    // TESTING ROOMS
    HouseType house;
    initHouse(&house, el);
    populateRooms(&house);

    int rand = randInt(0, (&(&house)->rooms)->size);
    RoomNodeType *n = (&(&house)->rooms)->head;
    while (rand > 0) {
        n = n->next;
        rand--;
    }
    

    // TESTING GHOSTS
    GhostType* ghost;
    // initGhost(randomGhost(), n->room, &ghost);
    // l_ghostInit(ghost->ghostType, n->room->name);

    initGhost(randomGhost(), (&(&house)->rooms)->head->next->room, &ghost);
    l_ghostInit(ghost->ghostType, (&(&house)->rooms)->head->next->room->name);


    // TESTING HUNTERS
    char hunterName[MAX_STR];
    HunterType *hunters[NUM_HUNTERS];
    
    for (int i = 0; i < NUM_HUNTERS; i++){
        printf("\nEnter name of Hunter %d: ", i+1);
        scanf("%s", hunterName);
        l_hunterInit(hunterName, i);
        initHunter(&hunters[i], hunterName, i, (&house)->evidenceCollection, (&(&house)->rooms)->head->room); 
    }
    printf("\n");
    printHunters(hunters);
    
    //for (int j = 0; j < NUM_HUNTERS; j++) {
    //   printHunters(hunters);
    //}
        moveHunterToRoom(hunters[0]);
        printf("%s\n",hunters[0]->room->name);

        moveHunterToRoom(hunters[1]);
        printf("%s\n",hunters[1]->room->name);
        
        moveHunterToRoom(hunters[2]);
        printf("%s\n",hunters[2]->room->name);
        
        moveHunterToRoom(hunters[3]);
        printf("%s\n",hunters[3]->room->name);
  
        leaveEvidence(ghost->room, 0);
        ghostMove(ghost);
        for (int z = 0; z < NUM_HUNTERS; z++) {
            hunterCollect(hunters[z]);
            printf("\n");
        }

        // test if evidence is shared
        for(int q = 0; q < NUM_HUNTERS; q++) {
            printf("%d\nhouse: %d\n",q, house.evidenceCollection[q]);
            printf("hunter1: %d\n",hunters[0]->evidenceCollection[q]);
            printf("hunter2: %d\n",hunters[1]->evidenceCollection[q]);
            printf("hunter3: %d\n",hunters[2]->evidenceCollection[q]);
            printf("hunter4: %d\n",hunters[3]->evidenceCollection[q]);

        }
        
    // int choice;
    // do {
    //     printMenu(&choice);

    //     switch(choice) {
    //         case 1:
    //             // print hunters
    //             printHunters(hunters);
    //             break;
    //         case 2:
    //             // print ghosts
    //             printGhosts(ghost, 5);
    //             break;
    //         case 3:
    //             // add hunters
    //             break;
    //         case 4:
    //             // review evidence
    //             for (int i = 0; i < NUM_HUNTERS; i++){
    //                 reviewEvidence(hunters[i]);
    //             }
    //             break;
    //         case 0:
    //             printf("Exiting the program....\n");
    //             break;
    //         default:
    //             printf("Invalid option. Please enter a valid menu option.\n");
    //     }
    // } while (choice != 0);


    // free ghost memory
    free(ghost);

    // free hunter memory
    for (int i = 0; i < NUM_HUNTERS; ++i){
        free(hunters[i]);
    }

    // // free allocated memory for rooms
    // RoomNodeType* currentRoomNode = rl->head;
    // while (currentRoomNode != NULL){
    //     RoomNodeType* nextRoomNode = currentRoomNode->next;

    //     free(currentRoomNode->room);
    //     free(currentRoomNode);

    //     currentRoomNode = nextRoomNode;
    // }
    
    // free(rl);

    return 0;
}



void printMenu(int *choice){
    printf("\nMAIN MENU\n");
    printf("  (1) Print Hunters\n");
    printf("  (2) Print Ghosts\n");
    printf("  (3) Add Hunters\n");
    printf("  (4) Review Evidence\n");
    printf("  (0) Exit\n\n");

    printf("Please enter your selection: ");
    scanf("%2d", choice);
    while (getchar() != '\n');
}