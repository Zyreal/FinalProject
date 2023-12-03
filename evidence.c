#include "defs.h"

/*
  Function: Initialize EvidenceList
  Purpose:  Initializes all fields of a EvidenceListType
  Params:
      Output: EvidenceListType* el - EvidenceListType to be initialized
*/
void initEvidenceList(EvidenceListType* el) {
    el->head = NULL;
    el->tail = NULL;
    el->size = 0;
}

/*
  Function: Cleanup Evidence
  Purpose:  Frees all allocated data of a EvidenceListType
  Params:
      Input/Output: EvidenceListType* el - EvidenceListType to be freed
*/
void cleanupEvidence(EvidenceListType* el) {
    EvidenceNodeType *head = el->head;
    EvidenceNodeType *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}